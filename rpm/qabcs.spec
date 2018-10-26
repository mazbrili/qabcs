Name: qabcs
Summary: Learn the English alphabet
Version: 1.0.0
Release: %mkrel 1
License: GPLv3+
Group: Education
URL: https://bitbucket.org/admsasha/qabcs

BuildRequires:  qttools5
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Widgets)
BuildRequires:  pkgconfig(Qt5Multimedia)
Requires:  espeak

Source0: %{name}-%{version}.tar.gz

%description
QABCs Educational Software. An educational game for young children.
QABCs familiarizes children with the alphabet and a keyboard.
Children can play with QABCs to learn about the alphabet and words
associated with it.

How to play:
* In ABC mode, press the key on the keyboard that is shown on the screen
to advance through the alphabet, press "Backspace" to go back
* In any other mode, press a key on the keyboard to display that letter
* To hear the word displayed on the screen, press the spacebar

%prep
%setup -q -n %{name}-%{version}

%build
%qmake_qt5
%make_build

%install
%make_install INSTALL_ROOT=$RPM_BUILD_ROOT

%files
%doc README
%license COPYING
%{_datadir}/%{name}
%{_bindir}/%{name}
%{_datadir}/pixmaps/%{name}.xpm
%{_datadir}/applications/%{name}.desktop
