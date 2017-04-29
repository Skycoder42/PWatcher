# Maintainer: Felix Barz <skycoder42.de@gmx.de>
pkgname=pwatcher
pkgver=1.0.0
pkgrel=2
pkgdesc="PWatcher + qt apng image plugin"
arch=('i686' 'x86_64')
url="https://github.com/Skycoder42/pwatcher"
license=('BSD')
depends=('qt5-base' 'qt5-tools')
optdepends=('qt5-imageformats: Support for additional icon types'
            'qt5-svg: Support for SVG-Icons')
source=('pwatcher'
		'libqapng.so'
		'de.skycoder42.pwatcher.desktop'
		'pwatcher_32.png'
		'pwatcher_16.png')
md5sums=('bf2854b7be3e6671cfa9d7f70d4f0ae2'
         'b46bba6607dfff5ec5626363ab42fe85'
         '06207a81e7afe0bcd7ad1413f0ec33ab'
         '5b5bd7885762ab9e1c73574cd4109fbf'
         '3c009f8e7113a7c40124eef99e536073')

package() {
	PLUGIN_DIR=$(qmake -query QT_INSTALL_PLUGINS)

	install -D -m 755 -p pwatcher $pkgdir/usr/bin/pwatcher
	install -D -m 755 -p libqapng.so $pkgdir/$PLUGIN_DIR/imageformats/libqapng.so
	install -D -m 644 -p de.skycoder42.${pkgname}.desktop $pkgdir/usr/share/applications/de.skycoder42.${pkgname}.desktop
	install -D -m 644 -p ${pkgname}_32.png "$pkgdir/usr/share/icons/hicolor/32x32/apps/$pkgname.png"
	install -D -m 644 -p ${pkgname}_16.png "$pkgdir/usr/share/icons/hicolor/16x16/apps/$pkgname.png"
}
