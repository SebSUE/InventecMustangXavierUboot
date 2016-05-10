### HEADER SECTION BELOW
Name:          u-boot
Version:       2015.01
Release:       1%{?dist}
Summary:       Das U-Boot
Group:         System/Boot
License:       GPL
URL:           http://www.denx.de/wiki/U-Boot
Source0:       %{name}-%{version}.tar.gz
ExclusiveArch: i586
BuildRequires: openssl-devel dtc
BuildRequires: cross-armv7l-gcc49 cross-armv7l-binutils
%description
Universal Boot Loader


%{lua:
   targets = {"bcm958305_hoka", "bcm958305_hpka", "bcm958305_hoka-ns",
   "bcm958305_hpka-ns", "bcm958305_hota", "bcm958305_hota-ns"}
   for i, target in ipairs(targets)
   do
      -- %package target
      -- Summary:    Das U-Boot
      -- Group:      System/Boot
      -- %description target
      -- Das U-Boot -- the Universal Boot Loader for target
      print("%package " .. target .. "\n")
      print("Summary:    Das U-Boot\n")
      print("Group:      System/Boot\n")
      print("%description " .. target .. "\n")
      print("Universal Boot Loader for " .. target .. "\n")
      print("\n")
   end
}


### PREP SECTION BELOW
%prep
%setup -q -n %{name}-%{version}


### BUILD SECTION BELOW
%build
# Clear out legacy FLAGS from Trunk:MxC (Meego)
export CFLAGS=
export CXXFLAGS=
export FFLAGS=
export CROSS_COMPILE=/opt/cross/bin/armv7l-meego-linux-gnueabi-
export PATH=/opt/cross/bin:$PATH

# CROSS_COMPILE setup
${CROSS_COMPILE}gcc --version

# Build commands
#CROSS_COMPILE=/opt/cross/bin/armv7l-meego-linux-gnueabi- ./MAKEALL -v broadcom -a arm
%{lua:
   for i, target in ipairs(targets)
   do
      -- make mrproper
      -- make $target_defconfig
      -- make
      print("echo -e '\n####################\n# " .. target .. "\n####################\n'\n")
      print("make mrproper\n")
      print("make " .. target .. "_defconfig\n")
      print("sleep 1\n")
      print("make\n")
      print("if [ -f u-boot.img ]; then\n")
      print("cp u-boot.img bin.u-boot-" .. target .. "\n")
      print("else\n")
      print("cp u-boot.bin bin.u-boot-" .. target .. "\n")
      print("fi\n")
      print("echo '/boot/bin.u-boot-" .. target .. "' > extra-files-" .. target .. "\n")
      print("if [ -f u-boot.dtb ]; then\n")
      print("cp u-boot.dtb bin.dtb-u-boot-" .. target .. "\n")
      print("echo '/boot/bin.dtb-u-boot-" .. target .. "' >> extra-files-" .. target .. "\n")
      print("fi\n")
   end
}


### INSTALL SECTION BELOW
%install
install -d %{buildroot}/boot
cp -pv bin.u-boot-* %{buildroot}/boot/
cp -pv bin.dtb-u-boot* %{buildroot}/boot/

# RPM package arch convert
%{lua:
   local sourcedir = rpm.expand("%{_sourcedir}")
   local basename = rpm.expand("%{name}")

   -- Prepare a clean baselibs.conf (used for i586->arm package conversions)
   print("rm -rf " .. sourcedir .. "/baselibs.conf\n")
   print("echo \"arch i586 targets armv7l:arm\" >> " .. sourcedir .. "/baselibs.conf\n")

   for i, package in ipairs(targets)
   do
      -- Add an entry into baselibs.conf
      -- Each entry has the format:
      --    PACKAGE-NAME
      --      targetname <name>
      --      targettype x86 block!
      --      targettype 32bit block!
      --      targettype arm autoreqprov off
      --      targettype arm requires "PACKAGE-NAME"
      --      targettype arm extension -arm
      --      targettype arm +/
      print("echo \"" .. basename .. "-".. package .. "\" >> " .. sourcedir .. "/baselibs.conf\n")
      print("echo \"  targetname <name>\" >> " .. sourcedir .. "/baselibs.conf\n")
      print("echo \"  targettype x86 block!\" >> " .. sourcedir .. "/baselibs.conf\n")
      print("echo \"  targettype 32bit block!\" >> " .. sourcedir .. "/baselibs.conf\n")
      print("echo \"  targettype arm autoreqprov off\" >> " .. sourcedir .. "/baselibs.conf\n")
      print("echo \"  targettype arm requires \\\"" .. basename .. "-" .. package .. "\\\"\" >> " .. sourcedir .. "/baselibs.conf\n")
      print("echo \"  targettype arm extension -arm\" >> " .. sourcedir .. "/baselibs.conf\n")
      print("echo \"  targettype arm +/\" >> " .. sourcedir .. "/baselibs.conf\n")
      print("\n")
   end
}
cat %{_sourcedir}/baselibs.conf


### CLEAN SECTION BELOW
%clean
rm -rf %{buildroot}


### FILES SECTION BELOW
%{lua:
   for i, target in ipairs(targets)
   do
      -- %files target
      -- %defattr(644,root,root,755)
      -- %attr(755,root,root) /boot/bin.u-boot-target
      print("%files " .. target .. " -f extra-files-" .. target .. "\n")
      print("%defattr(644,root,root,755)\n")
      -- print("%attr(755,root,root) /boot/bin.u-boot-" .. target .. "\n")
      print("\n")
   end
}


### CHANGELOG SECTION BELOW
%changelog

