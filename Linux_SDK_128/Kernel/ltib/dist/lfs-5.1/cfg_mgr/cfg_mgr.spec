%define pfx /opt/freescale/rootfs/%{_target_cpu}
%define __os_install_post %{nil}

Summary         : config manager
Name            : cfg_mgr
Version         : 1.0
Release         : 1
License         : XXXX
Vendor          : VVDN
Packager        : XXXX
Group           : XXXX
Source          : %{name}-%{version}.tar.gz
BuildRoot       : %{_tmppath}/%{name}
Prefix          : %{pfx}

%Description
%{summary}

%Prep
%setup 

%Build
export LDFLAGS="-L${DEV_IMAGE}/lib -L${DEV_IMAGE}/usr/lib"
make -C src CC="${TOOLCHAIN_PREFIX}gcc"
unset LDFLAGS

%Install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/%{pfx}/usr/bin
cp -f src/cfg_mgr $RPM_BUILD_ROOT/%{pfx}/usr/bin

%Clean
rm -rf $RPM_BUILD_ROOT

%Files
%defattr(-,root,root)
%{pfx}/*

