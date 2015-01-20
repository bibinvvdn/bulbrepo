%define pfx /opt/freescale/rootfs/%{_target_cpu}
%define __os_install_post %{nil}

Summary         : webserver manager
Name            : webs_mgr
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
mkdir -p $RPM_BUILD_ROOT/%{pfx}/var/www/cgi-bin
mkdir -p $RPM_BUILD_ROOT/%{pfx}/var/www/html
mkdir -p $RPM_BUILD_ROOT/%{pfx}/usr/bin
cp -f cgi-bin/*.cgi $RPM_BUILD_ROOT/%{pfx}/var/www/cgi-bin/
#cp -f cgi-bin/show_mac $RPM_BUILD_ROOT/%{pfx}/usr/bin/
cp -fr html/* $RPM_BUILD_ROOT/%{pfx}/var/www/html/

%Clean
rm -rf $RPM_BUILD_ROOT

%Files
%defattr(-,root,root)
%{pfx}/*

