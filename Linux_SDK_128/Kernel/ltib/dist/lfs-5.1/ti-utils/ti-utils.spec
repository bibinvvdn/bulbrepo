
%define pfx /opt/freescale/rootfs/%{_target_cpu}

Summary         : ti utility
Name            : ti-utils
Version         : 1.0
Release         : 1
License         : GPL
Vendor          : Freescale
Packager        : John Faith
Group           : Development/Tools
Source          : %{name}.tar.gz
BuildRoot       : %{_tmppath}/%{name}
Prefix          : %{pfx}

%Description
%{summary}

%Prep
%setup 

%Build
make

%Install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT/%{pfx}

%Clean
rm -rf $RPM_BUILD_ROOT

%Files
%defattr(-,root,root)
%{pfx}/*

