Summary: wmsensors draws graphs of data using CPU sensors
%define version 1.0.4
Name: wmsensors
Version: %{version}
Release: 1
Copyright: GPL
Group: X Windows/Window Managers
Source: http://users.ox.ac.uk/~kebl0850/wmlm78/%{name}-%{version}.tar.gz
Packager: Adrian Baugh <adrian.baugh@keb.ox.ac.uk>
BuildRoot: /var/tmp/%{name}-root
Requires: lm_sensors >= 2.0

%description
This application docks into the Window Maker dock and draws graphs of data
obtained from the sensors kernel module via a userspace library.

%prep
cd /usr/src/redhat/BUILD
rm -rf %{name}
tar zxvf ../SOURCES/%{name}-%{version}.tar.gz

%build
cd /usr/src/redhat/BUILD/wmsensors
xmkmf
make

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/X11R6/{bin,man/man1}
cd /usr/src/redhat/BUILD/wmsensors
make DESTDIR=$RPM_BUILD_ROOT/usr/X11R6 install
install wmsensors.1x $RPM_BUILD_ROOT/usr/X11R6/man/man1

%files
%defattr(-,root,root)
/usr/X11R6/bin/%{name}
/usr/X11R6/man/man1/%{name}.1x
%doc COPYING FAQ INSTALL TODO sensor-modules.README wmsensors.README

%clean
rm -rf $RPM_BUILD_ROOT

%changelog
* Thu Dec 03 1999 Adrian Baugh <adrian.baugh@keb.ox.ac.uk>

- updated to wmsensors-1.0.4

* Mon May 17 1999 Adrian Baugh <adrian.baugh@keb.ox.ac.uk>

- updated to wmsensors-1.0.3; removed an obsolete reference (sensors.less.sh)

* Thu Feb 25 1999 Ian Macdonald <ianmacd@xs4all.nl>

- first RPM release

