subdir-ccflags-y += -Wall -Werror

ifeq ($(CONFIG_HIFI_MAILBOX), y)
EXTRA_CFLAGS += -I$(srctree)/drivers/hisi/hifi_mailbox/mailbox
EXTRA_CFLAGS += -I$(srctree)/drivers/hisi/hifi_mailbox/ipcm
EXTRA_CFLAGS += -I$(srctree)/drivers/hisi/hifi_dsp
endif

EXTRA_CFLAGS +=	-Idrivers/hisi/slimbus/
EXTRA_CFLAGS +=	-Iinclude/linux/hisi/hi64xx/

snd-soc-hi3650-objs := hi6210_pcm.o slimbus_dai.o

obj-$(CONFIG_SND_SOC_PCM_HI3650) += snd-soc-hi3650.o

# HI3650-HI6402 Machine Support
snd-soc-hi3650-hi6402-objs := hi3650_hi6402.o

obj-$(CONFIG_SND_HI3650_HI6402) += snd-soc-hi3650-hi6402.o
