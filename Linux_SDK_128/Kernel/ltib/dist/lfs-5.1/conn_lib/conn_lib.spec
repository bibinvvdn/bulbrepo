%define pfx /opt/freescale/rootfs/%{_target_cpu}
%define __os_install_post %{nil}

Summary         : connection library
Name            : conn_lib
Version         : 1.0
Release         : 1
License         : XXXX
Vendor          : Valiant
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
make -C src CC="${TOOLCHAIN_PREFIX}gcc" AR="${TOOLCHAIN_PREFIX}ar"

%Install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/%{pfx}/usr/lib
cp -f src/libconn.a $RPM_BUILD_ROOT/%{pfx}/usr/lib

%Clean
rm -rf $RPM_BUILD_ROOT

%Files
%defattr(-,root,root)
%{pfx}/*

