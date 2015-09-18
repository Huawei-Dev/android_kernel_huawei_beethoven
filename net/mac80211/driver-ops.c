/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <net/mac80211.h>
#include "ieee80211_i.h"
#include "trace.h"
#include "driver-ops.h"

int drv_add_interface(struct ieee80211_local *local,
		      struct ieee80211_sub_if_data *sdata)
{
	int ret;

	might_sleep();

	if (WARN_ON(sdata->vif.type == NL80211_IFTYPE_AP_VLAN ||
		    (sdata->vif.type == NL80211_IFTYPE_MONITOR &&
		     !ieee80211_hw_check(&local->hw, WANT_MONITOR_VIF) &&
		     !(sdata->u.mntr_flags & MONITOR_FLAG_ACTIVE))))
		return -EINVAL;

	trace_drv_add_interface(local, sdata);
	ret = local->ops->add_interface(&local->hw, &sdata->vif);
	trace_drv_return_int(local, ret);

	if (ret == 0)
		sdata->flags |= IEEE80211_SDATA_IN_DRIVER;

	return ret;
}

int drv_change_interface(struct ieee80211_local *local,
			 struct ieee80211_sub_if_data *sdata,
			 enum nl80211_iftype type, bool p2p)
{
	int ret;

	might_sleep();

	if (!check_sdata_in_driver(sdata))
		return -EIO;

	trace_drv_change_interface(local, sdata, type, p2p);
	ret = local->ops->change_interface(&local->hw, &sdata->vif, type, p2p);
	trace_drv_return_int(local, ret);
	return ret;
}

void drv_remove_interface(struct ieee80211_local *local,
			  struct ieee80211_sub_if_data *sdata)
{
	might_sleep();

	if (!check_sdata_in_driver(sdata))
		return;

	trace_drv_remove_interface(local, sdata);
	local->ops->remove_interface(&local->hw, &sdata->vif);
	sdata->flags &= ~IEEE80211_SDATA_IN_DRIVER;
	trace_drv_return_void(local);
}

__must_check
int drv_sta_state(struct ieee80211_local *local,
		  struct ieee80211_sub_if_data *sdata,
		  struct sta_info *sta,
		  enum ieee80211_sta_state old_state,
		  enum ieee80211_sta_state new_state)
{
	int ret = 0;

	might_sleep();

	sdata = get_bss_sdata(sdata);
	if (!check_sdata_in_driver(sdata))
		return -EIO;

	trace_drv_sta_state(local, sdata, &sta->sta, old_state, new_state);
	if (local->ops->sta_state) {
		ret = local->ops->sta_state(&local->hw, &sdata->vif, &sta->sta,
					    old_state, new_state);
	} else if (old_state == IEEE80211_STA_AUTH &&
		   new_state == IEEE80211_STA_ASSOC) {
		ret = drv_sta_add(local, sdata, &sta->sta);
		if (ret == 0)
			sta->uploaded = true;
	} else if (old_state == IEEE80211_STA_ASSOC &&
		   new_state == IEEE80211_STA_AUTH) {
		drv_sta_remove(local, sdata, &sta->sta);
	}
	trace_drv_return_int(local, ret);
	return ret;
}

void drv_sta_rc_update(struct ieee80211_local *local,
		       struct ieee80211_sub_if_data *sdata,
		       struct ieee80211_sta *sta, u32 changed)
{
	sdata = get_bss_sdata(sdata);
	if (!check_sdata_in_driver(sdata))
		return;

	WARN_ON(changed & IEEE80211_RC_SUPP_RATES_CHANGED &&
		(sdata->vif.type != NL80211_IFTYPE_ADHOC &&
		 sdata->vif.type != NL80211_IFTYPE_MESH_POINT));

	trace_drv_sta_rc_update(local, sdata, sta, changed);
	if (local->ops->sta_rc_update)
		local->ops->sta_rc_update(&local->hw, &sdata->vif,
					  sta, changed);

	trace_drv_return_void(local);
}

int drv_conf_tx(struct ieee80211_local *local,
		struct ieee80211_sub_if_data *sdata, u16 ac,
		const struct ieee80211_tx_queue_params *params)
{
	int ret = -EOPNOTSUPP;

	might_sleep();

	if (!check_sdata_in_driver(sdata))
		return -EIO;

	if (WARN_ONCE(params->cw_min == 0 ||
		      params->cw_min > params->cw_max,
		      "%s: invalid CW_min/CW_max: %d/%d\n",
		      sdata->name, params->cw_min, params->cw_max))
		return -EINVAL;

	trace_drv_conf_tx(local, sdata, ac, params);
	if (local->ops->conf_tx)
		ret = local->ops->conf_tx(&local->hw, &sdata->vif,
					  ac, params);
	trace_drv_return_int(local, ret);
	return ret;
}
