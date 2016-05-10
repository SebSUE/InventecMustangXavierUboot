### HEADER SECTION BELOW
Name:          bcm-u-boot-tools
Version:       2015.01
Release:       1%{?dist}
Summary:       Das U-Boot
Group:         System/Boot
License:       GPL
URL:           http://www.denx.de/wiki/U-Boot
Source0:       %{name}-%{version}.tar.gz
BuildRequires:  zlib-devel openssl-devel
Requires:       openssl, dtc
Provides:       u-boot-tools
# Obsoletes packages from upstream (u-boot-tools) and old Trunk:MxC:Tools
Obsoletes:      u-boot-tools, uboot-mkimage, ep-u-boot-tools

%description
Universal Boot Loader host tools

%prep
%setup -q -n %{name}-%{version}

%build
make distclean
make sandbox_defconfig NO_SDL=1
sleep 1
make tools

%install
install -d %{buildroot}/usr/bin
cp -pv tools/mkimage %{buildroot}/usr/bin

%files
%defattr(644,root,root,755)
%attr(755,root,root) /usr/bin/mkimage


%clean
rm -rf %{buildroot}


%changelog

