###Preparing OpenWrt build system for use
---

1 link git clone https://git.openwrt.org/openwrt/openwrt.git source
* cd source
* git checkout v18.06.4
* make distclean
* ./scripts/feeds update -a
* ./scripts/feeds install -a
* make menuconfig
	* choose the suitable 'Target System', 'Subtarget' and 'Target Profile' -> save
* make toolchain/install ( it is going to take a while)
* export PATH=/home/vagrant/buildbot/source/staging_dir/host/bin:$PATH
* make package/mosquitto/compile
* ./scripts/feeds update mypackages
* ./scripts/feeds install -a -p mypackages
------
###Add to Makefile package
---
<pre>
PKG_BUILD_DEPENDS:=libopenssl
</pre>
<pre>
define Package/toolsense_owrt
	...
	DEPENDS:=+libmosquitto
	...
endef
</pre>
<pre>
$(eval $(call BuildPackage,toolsense_owrt,+libmosquitto))
</pre>
