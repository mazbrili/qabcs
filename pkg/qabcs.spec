Name:           qabcs
Version:        1.0.0
Release:        %mkrel 1
Summary:        Learn alphabet
Group:          Education
License:        GPLv3+
Url:            https://bitbucket.org/admsasha/qabcs
Source0:        https://bitbucket.org/admsasha/kroots/downloads/%{name}-%{version}.tar.gz

BuildRequires:  qttools5
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Widgets)
BuildRequires:  pkgconfig(Qt5Multimedia)
Requires:       espeak

%description
qABCs is Educational Software. An educational game for young children.
qABCs familiarizes children with the alphabet and a keyboard.
Children can play with qABCs to learn about the alphabet and words
associated with it.

How to play:
* In ABC mode, press the key on the keyboard that is shown on the screen
to advance through the alphabet, press "Backspace" to go back
* In any other mode, press a key on the keyboard to display that letter
* To hear the word displayed on the screen, press the spacebar

%prep
%setup -q

%build
%qmake_qt5
%make_build

%install
%make_install INSTALL_ROOT=%{buildroot}

%files
%doc README
%license COPYING
%{_datadir}/%{name}
%{_bindir}/%{name}
%{_datadir}/pixmaps/%{name}.xpm
%{_datadir}/applications/%{name}.desktop
%{_mandir}/man1/*.1*
