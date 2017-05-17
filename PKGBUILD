# Maintainer: Felix Barz <skycoder42.de@gmx.de>
pkgname=pwatcher
pkgver=1.1.0
pkgrel=1
pkgdesc="PWatcher Diashow Tool"
arch=('i686' 'x86_64')
url="https://github.com/Skycoder42/$pkgname"
license=('BSD')
depends=('qt5-base' 'qt5-tools')
makedepends=('git' 'qpm')
optdepends=('qt5-imageformats: Support for additional icon types'
			'qt5-svg: Support for SVG-Icons'
			'apng-project: Support for APNG-Files')
source=("$pkgname-$pkgver::git+https://github.com/Skycoder42/$pkgname.git#branch=rebuild") #tag=$pkgver")
md5sums=('SKIP')

build() {
  cd "$pkgname-$pkgver"

  qpm install
  qmake
  make qmake_all
  make
}

package() {
  cd "$pkgname-$pkgver"

  make INSTALL_ROOT="$pkgdir" install

  install -D -m644 de.skycoder42.${pkgname}.desktop "$pkgdir/usr/share/applications/de.skycoder42.${pkgname}.desktop"
  install -D -m644 ${pkgname}_32.png "$pkgdir/usr/share/icons/hicolor/32x32/apps/$pkgname.png"
  install -D -m644 ${pkgname}_16.png "$pkgdir/usr/share/icons/hicolor/16x16/apps/$pkgname.png"
  install -D -m644 LICENSE "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
}
