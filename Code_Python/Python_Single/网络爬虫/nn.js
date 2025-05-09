$(function () {
    var $bookcnt = $('#J_BookCnt');
    var $bookread = $('#J_BookRead');
    var $shezhi = $('#J_CntShezhi');
    var $mulu = $('#J_CntMulu');
    var $submit = $shezhi.find('.J_Submit');
    var $cancel = $shezhi.find('.J_Cancel');
    var $theme = $shezhi.find('#J_DlTheme');
    var $fontstyle = $shezhi.find('#J_DlFontStyle');
    var $fontfamily = $shezhi.find('#J_DlFontFamily');
    var $fontsize = $shezhi.find('#J_DlFontSize');
    var $getContentUrl = 'chapter/get_book_chapter_detail_info';
    var $getImageTsukkomi = 'chapter/chapter_image_tsukkomi_list';
    var $getImageHeightUrl = 'chapter/get_book_chapter_image_height';
    var $tsukkomiSite = $shezhi.find('#J_TsukkomiSite');
    var $musicPlayer = $shezhi.find('#J_MusicPlayer');
    var barrageInstance;
    var preParentWidth = 0;
    var preSetColorName = '';
    var preSetFontIndex = 0;
    var preSetFontSize = 0;
    var STR = {
        hexcase: 0,
        b64pad: '',
        chrsz: 8,
        b64_hmac_md5: function (key, data) {
            return binl2b64(core_hmac_md5(key, data));
        },
        b64_md5: function (s) {
            return binl2b64(core_md5(str2binl(s), s.length * this.chrsz));
        },
        binl2b64: function (binarray) {
            var tab = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/';
            var str = '';
            for (var i = 0; i < binarray.length * 4; i += 3) {
                var triplet = (binarray[i >> 2] >> 8 * (i % 4) & 255) << 16 | (binarray[i + 1 >> 2] >> 8 * ((i + 1) % 4) & 255) << 8 | binarray[i + 2 >> 2] >> 8 * ((i + 2) % 4) & 255;
                for (var j = 0; j < 4; j++) {
                    if (i * 8 + j * 6 > binarray.length * 32) str += this.b64pad;
                    else str += tab.charAt(triplet >> 6 * (3 - j) & 63);
                }
            }
            return str;
        },
        binl2hex: function (binarray) {
            var hex_tab = this.hexcase ? '0123456789ABCDEF' : '0123456789abcdef';
            var str = '';
            for (var i = 0; i < binarray.length * 4; i++) {
                str += hex_tab.charAt(binarray[i >> 2] >> i % 4 * 8 + 4 & 15) + hex_tab.charAt(binarray[i >> 2] >> i % 4 * 8 & 15);
            }
            return str;
        },
        binl2str: function (bin) {
            var str = '';
            var mask = (1 << this.chrsz) - 1;
            for (var i = 0; i < bin.length * 32; i += this.chrsz) str += String.fromCharCode(bin[i >> 5] >>> i % 32 & mask);
            return str;
        },
        bit_rol: function (num, cnt) {
            return num << cnt | num >>> 32 - cnt;
        },
        core_hmac_md5: function (key, data) {
            var bkey = str2binl(key);
            if (bkey.length > 16) bkey = core_md5(bkey, key.length * this.chrsz);
            var ipad = Array(16),
                opad = Array(16);
            for (var i = 0; i < 16; i++) {
                ipad[i] = bkey[i] ^ 909522486;
                opad[i] = bkey[i] ^ 1549556828;
            }
            var hash = core_md5(ipad.concat(str2binl(data)), 512 + data.length * this.chrsz);
            return core_md5(opad.concat(hash), 512 + 128);
        },
        core_md5: function (x, len) {
            x[len >> 5] |= 128 << len % 32;
            x[(len + 64 >>> 9 << 4) + 14] = len;
            var a = 1732584193;
            var b = - 271733879;
            var c = - 1732584194;
            var d = 271733878;
            for (var i = 0; i < x.length; i += 16) {
                var olda = a;
                var oldb = b;
                var oldc = c;
                var oldd = d;
                a = this.md5_ff(a, b, c, d, x[i + 0], 7, - 680876936);
                d = this.md5_ff(d, a, b, c, x[i + 1], 12, - 389564586);
                c = this.md5_ff(c, d, a, b, x[i + 2], 17, 606105819);
                b = this.md5_ff(b, c, d, a, x[i + 3], 22, - 1044525330);
                a = this.md5_ff(a, b, c, d, x[i + 4], 7, - 176418897);
                d = this.md5_ff(d, a, b, c, x[i + 5], 12, 1200080426);
                c = this.md5_ff(c, d, a, b, x[i + 6], 17, - 1473231341);
                b = this.md5_ff(b, c, d, a, x[i + 7], 22, - 45705983);
                a = this.md5_ff(a, b, c, d, x[i + 8], 7, 1770035416);
                d = this.md5_ff(d, a, b, c, x[i + 9], 12, - 1958414417);
                c = this.md5_ff(c, d, a, b, x[i + 10], 17, - 42063);
                b = this.md5_ff(b, c, d, a, x[i + 11], 22, - 1990404162);
                a = this.md5_ff(a, b, c, d, x[i + 12], 7, 1804603682);
                d = this.md5_ff(d, a, b, c, x[i + 13], 12, - 40341101);
                c = this.md5_ff(c, d, a, b, x[i + 14], 17, - 1502002290);
                b = this.md5_ff(b, c, d, a, x[i + 15], 22, 1236535329);
                a = this.md5_gg(a, b, c, d, x[i + 1], 5, - 165796510);
                d = this.md5_gg(d, a, b, c, x[i + 6], 9, - 1069501632);
                c = this.md5_gg(c, d, a, b, x[i + 11], 14, 643717713);
                b = this.md5_gg(b, c, d, a, x[i + 0], 20, - 373897302);
                a = this.md5_gg(a, b, c, d, x[i + 5], 5, - 701558691);
                d = this.md5_gg(d, a, b, c, x[i + 10], 9, 38016083);
                c = this.md5_gg(c, d, a, b, x[i + 15], 14, - 660478335);
                b = this.md5_gg(b, c, d, a, x[i + 4], 20, - 405537848);
                a = this.md5_gg(a, b, c, d, x[i + 9], 5, 568446438);
                d = this.md5_gg(d, a, b, c, x[i + 14], 9, - 1019803690);
                c = this.md5_gg(c, d, a, b, x[i + 3], 14, - 187363961);
                b = this.md5_gg(b, c, d, a, x[i + 8], 20, 1163531501);
                a = this.md5_gg(a, b, c, d, x[i + 13], 5, - 1444681467);
                d = this.md5_gg(d, a, b, c, x[i + 2], 9, - 51403784);
                c = this.md5_gg(c, d, a, b, x[i + 7], 14, 1735328473);
                b = this.md5_gg(b, c, d, a, x[i + 12], 20, - 1926607734);
                a = this.md5_hh(a, b, c, d, x[i + 5], 4, - 378558);
                d = this.md5_hh(d, a, b, c, x[i + 8], 11, - 2022574463);
                c = this.md5_hh(c, d, a, b, x[i + 11], 16, 1839030562);
                b = this.md5_hh(b, c, d, a, x[i + 14], 23, - 35309556);
                a = this.md5_hh(a, b, c, d, x[i + 1], 4, - 1530992060);
                d = this.md5_hh(d, a, b, c, x[i + 4], 11, 1272893353);
                c = this.md5_hh(c, d, a, b, x[i + 7], 16, - 155497632);
                b = this.md5_hh(b, c, d, a, x[i + 10], 23, - 1094730640);
                a = this.md5_hh(a, b, c, d, x[i + 13], 4, 681279174);
                d = this.md5_hh(d, a, b, c, x[i + 0], 11, - 358537222);
                c = this.md5_hh(c, d, a, b, x[i + 3], 16, - 722521979);
                b = this.md5_hh(b, c, d, a, x[i + 6], 23, 76029189);
                a = this.md5_hh(a, b, c, d, x[i + 9], 4, - 640364487);
                d = this.md5_hh(d, a, b, c, x[i + 12], 11, - 421815835);
                c = this.md5_hh(c, d, a, b, x[i + 15], 16, 530742520);
                b = this.md5_hh(b, c, d, a, x[i + 2], 23, - 995338651);
                a = this.md5_ii(a, b, c, d, x[i + 0], 6, - 198630844);
                d = this.md5_ii(d, a, b, c, x[i + 7], 10, 1126891415);
                c = this.md5_ii(c, d, a, b, x[i + 14], 15, - 1416354905);
                b = this.md5_ii(b, c, d, a, x[i + 5], 21, - 57434055);
                a = this.md5_ii(a, b, c, d, x[i + 12], 6, 1700485571);
                d = this.md5_ii(d, a, b, c, x[i + 3], 10, - 1894986606);
                c = this.md5_ii(c, d, a, b, x[i + 10], 15, - 1051523);
                b = this.md5_ii(b, c, d, a, x[i + 1], 21, - 2054922799);
                a = this.md5_ii(a, b, c, d, x[i + 8], 6, 1873313359);
                d = this.md5_ii(d, a, b, c, x[i + 15], 10, - 30611744);
                c = this.md5_ii(c, d, a, b, x[i + 6], 15, - 1560198380);
                b = this.md5_ii(b, c, d, a, x[i + 13], 21, 1309151649);
                a = this.md5_ii(a, b, c, d, x[i + 4], 6, - 145523070);
                d = this.md5_ii(d, a, b, c, x[i + 11], 10, - 1120210379);
                c = this.md5_ii(c, d, a, b, x[i + 2], 15, 718787259);
                b = this.md5_ii(b, c, d, a, x[i + 9], 21, - 343485551);
                a = this.safe_add(a, olda);
                b = this.safe_add(b, oldb);
                c = this.safe_add(c, oldc);
                d = this.safe_add(d, oldd);
            }
            return Array(a, b, c, d);
        },
        hex_hmac_md5: function (key, data) {
            return this.binl2hex(this.core_hmac_md5(key, data));
        },
        hex_md5: function (s) {
            return this.binl2hex(this.core_md5(this.str2binl(s), s.length * this.chrsz));
        },
        md5: function (s) {
            return this.hex_md5(s);
        },
        md5_cmn: function (q, a, b, x, s, t) {
            return this.safe_add(this.bit_rol(this.safe_add(this.safe_add(a, q), this.safe_add(x, t)), s), b);
        },
        md5_ff: function (a, b, c, d, x, s, t) {
            return this.md5_cmn(b & c | ~b & d, a, b, x, s, t);
        },
        md5_gg: function (a, b, c, d, x, s, t) {
            return this.md5_cmn(b & d | c & ~d, a, b, x, s, t);
        },
        md5_hh: function (a, b, c, d, x, s, t) {
            return this.md5_cmn(b ^ c ^ d, a, b, x, s, t);
        },
        md5_ii: function (a, b, c, d, x, s, t) {
            return this.md5_cmn(c ^ (b | ~d), a, b, x, s, t);
        },
        md5_vm_test: function () {
            return hex_md5('abc') == '900150983cd24fb0d6963f7d28e17f72';
        },
        safe_add: function (x, y) {
            var lsw = (x & 65535) + (y & 65535);
            var msw = (x >> 16) + (y >> 16) + (lsw >> 16);
            return msw << 16 | lsw & 65535;
        },
        str2binl: function (str) {
            var bin = Array();
            var mask = (1 << this.chrsz) - 1;
            for (var i = 0; i < str.length * this.chrsz; i += this.chrsz) bin[i >> 5] |= (str.charCodeAt(i / this.chrsz) & mask) << i % 32;
            return bin;
        },
        str_hmac_md5: function (key, data) {
            return binl2str(core_hmac_md5(key, data));
        },
        str_md5: function (s) {
            return binl2str(core_md5(str2binl(s), s.length * this.chrsz));
        }
    };
    function addSlash(str) {
        return str.replace(/\\/g, '\\\\').replace(/\//g, '\\/').replace(/\r/g, '\\r').replace(/\n/g, '\\n').replace(/\"/g, '\\"').replace(/\'/g, '\\\'').replace(/\t/g, '\\t');
    }
    function XOR(strV, strPass) {
        var intPassLength = strPass.length;
        var re = '';
        for (var i = 0; i < strV.length; i++) {
            re += String.fromCharCode(strV.charCodeAt(i) ^ strPass.charCodeAt(i % intPassLength));
        }
        return re;
    }
    function webEncode(messageInfo, pwd, id) {
        pwd = pwd + id;
        messageInfo = XOR(messageInfo, pwd);
        strSource = escape(XOR(messageInfo, STR.md5(pwd)));
        var strHTML = '';
        for (var i = 0; i + 70 < strSource.length; i += 70) {
            strHTML += addSlash(strSource.substring(i, i + 70));
        }
        strHTML += addSlash(strSource.substring(i, strSource.length));
        return strHTML;
    }
    if (window._phantom) {
        return;
    }
    var resizeId;
    if (typeof initResponse == 'function') {
        window.onresize = function () {
            initResponse();
            clearTimeout(resizeId);
            resizeId = setTimeout(changeBookImage, 500);
        };
    } else {
        window.onresize = function () {
            clearTimeout(resizeId);
            resizeId = setTimeout(changeBookImage, 500);
        };
    }
    if (0 == HB.config.bookread.useImage) {
        $.ajax({
            async: false,
            url: HB.config.rootPath + 'chapter/ajax_get_session_code',
            data: {
                chapter_id: HB.book.chapter_id
            },
            success: function (res) {
                HB.config.chapterAccessKey = res.chapter_access_key;
            }
        });
        $.ajax({
            async: false,
            url: HB.config.rootPath + $getContentUrl,
            data: {
                chapter_id: HB.book.chapter_id,
                chapter_access_key: HB.config.chapterAccessKey
            },
            success: function (res) {
                if (res.code == 100000) {
                    messageInfo = res.chapter_content;
                    rad = HB.book.chapter_id + '100000' + res.rad;
                    keys = res.encryt_keys;
                } else {
                    messageInfo = '';
                    keys = [
                    ];
                }
                $bookread.prepend($.myDecrypt({
                    content: messageInfo,
                    keys: keys,
                    accessKey: HB.config.chapterAccessKey
                }));
            }
        });
    } else {
    }
    function readBodyHeight() {
        var dis,
            read_bd = $('.read-bd'),
            read_bd_height = read_bd.height(),
            read_bd_wrap_height = $('.read-bd-wrap').height(),
            chapter_comment = $('.chapter-comment-wrap'),
            pos = chapter_comment.position(),
            posTop = pos.top,
            chapter_comment_height = chapter_comment.height(),
            totalHeight = posTop + chapter_comment_height;
        if (totalHeight > read_bd_height) {
            dis = totalHeight - read_bd_height;
            read_bd_height += dis;
            read_bd.height(read_bd_height);
        } else {
            read_bd.height(read_bd_wrap_height);
        }
    }
    function changeBookImage() {
        if ($('#J_BookImage').length > 0) {
            var parentWidth = $('#J_BookImage').width();
            var setColorName = $theme.find('.act').attr('data-value');
            var setFontIndex = $fontfamily.find('.act').attr('data-value');
            var setFontSize = $fontsize.find('span').text();
            if (preParentWidth == parentWidth && preSetColorName == setColorName && preSetFontIndex == setFontIndex && preSetFontSize == setFontSize) {
                return false;
            }
            preParentWidth = parentWidth;
            preSetColorName = setColorName;
            preSetFontIndex = setFontIndex;
            preSetFontSize = setFontSize;
            var imageCode = '';
            $.ajax({
                async: false,
                url: HB.config.rootPath + 'chapter/ajax_get_image_session_code',
                success: function (res) {
                    var messageInfo = '';
                    var keys = [
                    ];
                    var accessKey = '123';
                    if (res.code == 100000) {
                        messageInfo = res.image_code;
                        keys = res.encryt_keys;
                        accessKey = res.access_key;
                    }
                    imageCode = $.myDecrypt({
                        content: messageInfo,
                        keys: keys,
                        accessKey: accessKey
                    });
                }
            });
            var $bookImage = $('#realBookImage');
            var $bookImageDiv = $('#J_BookImage');
            $bookImageDiv.css('backgroundImage', 'url(' + HB.config.rootPath + 'chapter/book_chapter_image?chapter_id=' + HB.book.chapter_id + '&area_width=' + parentWidth + '&font=' + HB.config.bookread.fontsForImage[setFontIndex] + '&font_size=' + setFontSize + '&image_code=' + imageCode + '&bg_color_name=' + setColorName + '&text_color_name=' + setColorName + ')');
            $.ajax({
                async: false,
                url: HB.config.rootPath + $getImageHeightUrl + '?chapter_id=' + HB.book.chapter_id + '&area_width=' + parentWidth + '&font=' + HB.config.bookread.fontsForImage[setFontIndex] + '&font_size=' + setFontSize + '&bg_color_name=' + setColorName + '&text_color_name=' + setColorName,
                success: function (res) {
                    $bookImage.height(res + 'px');
                }
            });
            $.ajax({
                async: false,
                url: HB.config.rootPath + $getImageTsukkomi,
                data: {
                    chapter_id: HB.book.chapter_id,
                    area_width: parentWidth,
                    font: HB.config.bookread.fontsForImage[setFontIndex],
                    font_size: setFontSize
                },
                success: function (res) {
                    if (res.code == 100000) {
                        $('#J_BookImage').find('div.chapter-comment-wrap').remove();
                        $('#J_BookImage').find('i.J_Num').remove();
                        var setFontHeight = res.height;
                        var imageInfoMaps = res.imageInfoMaps;
                        var paddingTop = $bookImage.css('padding-top');
                        var matchPattern = /\d+/;
                        var matchResult = paddingTop.match(matchPattern);
                        var paddingTopHeight = 0;
                        var nowHeight = 0;
                        var tsukkomi_index = 0;
                        $.each(res.tsukkomi_list, function (i, item) {
                            tsukkomi_index = i;
                            if (imageInfoMaps[item[1]]) {
                                if (tsukkomi_num_json[i]) {
                                    topPx = parseFloat(nowHeight + imageInfoMaps[item[1]]['height'] / 2 + 0.5 * setFontHeight) - 13 + paddingTopHeight + 2;
                                    $('#J_BookImage').append('<i data-pgid=\'' + i + '\' class=\'J_Num num num1\' top=\'' + topPx + '\' style=\'top: ' + topPx + 'px\' >' + tsukkomi_num_json[i] + '</i>');
                                } else {
                                    topPx = parseFloat(nowHeight + imageInfoMaps[item[1]]['height'] / 2 + 0.5 * setFontHeight) - 8 + paddingTopHeight + 2;
                                    $('#J_BookImage').append('<i data-pgid=\'' + i + '\' class=\'J_Num num\' top=\'' + topPx + '\' style=\'top: ' + topPx + 'px\' ></i>');
                                }
                                nowHeight += imageInfoMaps[item[1]]['height'] + 1.2 * setFontHeight;
                            } else {
                                if (tsukkomi_num_json[i]) {
                                    topPx = parseFloat(nowHeight + (item[1] - item[0]) * 2.2 * setFontHeight + 0.5 * setFontHeight) - 13 + paddingTopHeight + 2;
                                    $('#J_BookImage').append('<i data-pgid=\'' + i + '\' class=\'J_Num num num1\' top=\'' + topPx + '\' style=\'top: ' + topPx + 'px\' >' + tsukkomi_num_json[i] + '</i>');
                                } else {
                                    topPx = parseFloat(nowHeight + (item[1] - item[0]) * 2.2 * setFontHeight + 0.5 * setFontHeight) - 8 + paddingTopHeight + 2;
                                    $('#J_BookImage').append('<i data-pgid=\'' + i + '\' class=\'J_Num num\' top=\'' + topPx + '\' style=\'top: ' + topPx + 'px\' ></i>');
                                }
                                nowHeight += (item[1] - item[0] + 1) * 2.2 * setFontHeight;
                            }
                        });
                        $('p.author_say').each(function (index) {
                            var i = index + 1 + parseInt(tsukkomi_index);
                            if (tsukkomi_num_json[i]) {
                                $(this).append('<i data-pgid=\'' + i + '\' class=\'J_Num num num1\'>' + tsukkomi_num_json[i] + '</i>');
                            } else {
                                $(this).append('<i data-pgid=\'' + i + '\' class=\'J_Num num\'></i>');
                            }
                        });
                    } else {
                        alert(res.error_message);
                    }
                }
            });
            var read_bd = $('.read-bd');
            read_bd.height('100%');
        }
    }
    function readSideBarSite() {
        var $readSideBar = $('#J_ReadSideBarBox'),
            top = 118;
        var $bar = $readSideBar.find('.read-side-bar');
        if ($(this).scrollTop() < top) {
            $bar.css({
                top: top - $(this).scrollTop() + 'px'
            });
        } else {
            $bar.css({
                top: 0
            });
        }
    }
    readSideBarSite();
    $(window).scroll(function () {
        readSideBarSite();
    });
    $('#J_ReadSideBarBox .btn').click(function () {
        var self = $(this);
        var $cntBox = $('#J_ReadSideBarCnt');
        var cnt = $(this).attr('data-cnt');
        if (cnt) {
            if (self.hasClass('active')) {
                self.removeClass('active');
                $cntBox.hide().find('.' + cnt).hide();
            } else {
                if (cnt == 'cnt-mulu') {
                    $.ajax({
                        url: HB.config.rootPath + 'chapter/get_chapter_list_in_chapter_detail',
                        data: {
                            book_id: HB.book.book_id,
                            chapter_id: HB.book.chapter_id
                        },
                        dataType: 'html',
                        success: function (res) {
                            $('#J_MuluScrollbar').empty().append(res);
                            self.addClass('active').siblings().removeClass('active');
                            $cntBox.show().find('.' + cnt).show().siblings().hide();
                            $('.' + cnt + ' #J_MuluScrollbar').animate({
                                scrollTop: $('.' + cnt + ' #J_MuluScrollbar .current') [0]['offsetTop']
                            }, 10);
                        }
                    });
                } else {
                    self.addClass('active').siblings().removeClass('active');
                    $cntBox.show().find('.' + cnt).show().siblings().hide();
                }
            }
        }
    });
    $mulu.find('.J_Close').click(function () {
        setTheme();
        setFontFamliy();
        setFontSize();
        setFontStyle();
        setTsukkomiSite();
        setMusicPlayer();
        setSubmitBtn();
        CloseReadSideBarBox();
    });
    $shezhi.find('.J_Close').click(function () {
        setTheme();
        setFontFamliy();
        setFontSize();
        setFontStyle();
        setTsukkomiSite();
        setMusicPlayer();
        setSubmitBtn();
        CloseReadSideBarBox();
    });
    $theme.find('a').click(function () {
        if ($(this).hasClass('act')) return;
        $(this).addClass('act').siblings().removeClass('act');
        themePreview($(this).attr('data-value'));
        setSubmitBtn();
    });
    $fontfamily.find('a').click(function () {
        if ($(this).hasClass('act')) return;
        $(this).addClass('act').siblings().removeClass('act');
        fontFamilyPreview($(this).attr('data-value'));
        setSubmitBtn();
    });
    $fontsize.find('a').click(function () {
        var fontSize = parseInt($fontsize.find('span').text());
        console.log(fontSize);
        if ($(this).hasClass('small') && fontSize > 12) {
            fontSize -= 2;
        }
        if ($(this).hasClass('big') && fontSize < 48) {
            fontSize += 2;
        }
        $fontsize.find('span').text(fontSize);
        fontSizePreview(fontSize);
        setSubmitBtn();
    });
    $fontstyle.find('a').click(function () {
        if ($(this).hasClass('act')) return;
        $(this).addClass('act').siblings().removeClass('act');
        fontStylePreview();
        setSubmitBtn();
    });
    $tsukkomiSite.find('a').click(function () {
        if ($(this).hasClass('act')) return;
        $(this).addClass('act').siblings().removeClass('act');
        tsukkomiPreview($(this).attr('data-value'));
        setSubmitBtn();
    });
    $tsukkomiSite.find('.tsu-hide').click(function () {
        $('.chapter-comment-wrap').prev().find('.J_Num').trigger('click');
        if ($('.chapter-comment-wrap').prev().hasClass('J_Num')) {
            $('.chapter-comment-wrap').prev().trigger('click');
        }
        $('#J_BarrageBox .btn-close').trigger('click');
    });
    $musicPlayer.find('a').click(function () {
        if ($(this).hasClass('act')) return;
        $(this).addClass('act').siblings().removeClass('act');
        musicPlayerPreview($(this).attr('data-value'));
        setSubmitBtn();
    });
    $submit.click(function () {
        HB.util.Cookie.set('bookReadTheme', $(this).attr('data-cookie'), {
            domain: document.domain,
            path: '/',
            expires: 365
        });
        CloseReadSideBarBox();
        var _cssDomain = '../css/';
        var _href = '';
        var _settings = HB.util.Cookie.get('bookReadTheme');
        if (_settings) {
            var _arr = _settings.split(',');
            if (_arr[0]) {
                _href += _cssDomain + 'skin/' + _arr[0] + '.css';
                HB.config.bookread.theme = _arr[0];
            }
            if (_arr[1]) {
                HB.config.bookread.fontFamily = HB.config.bookread.fonts[_arr[1]];
                HB.config.bookread.fontFamilyNum = _arr[1];
            }
            if (_arr[2]) {
                HB.config.bookread.fontSize = _arr[2];
            }
            if (_arr[3]) {
                HB.config.bookread.fontStyle = _arr[3];
            }
            if (_arr[4]) {
                HB.config.bookread.tsukkomiSite = _arr[4];
            }
            if (_arr[5]) {
                HB.config.bookread.musicPlayer = _arr[5];
            }
        }
        if (!_href) {
            _href = _cssDomain + 'skin/' + HB.config.bookread.theme + '.css';
        }
    });
    $cancel.click(function () {
        setTheme();
        setFontFamliy();
        setFontSize();
        setFontStyle();
        setTsukkomiSite();
        setMusicPlayer();
        setSubmitBtn();
        CloseReadSideBarBox();
    });
    function setTheme() {
        if (HB.config.bookread.theme) {
            $theme.find('.' + HB.config.bookread.theme).trigger('click');
        }
    }
    function setFontFamliy() {
        if (HB.config.bookread.fontFamily) {
            $fontfamily.find('a[data-value=\'' + HB.config.bookread.fontFamilyNum + '\']').trigger('click');
        }
    }
    function setFontSize() {
        if (HB.config.bookread.fontSize) {
            $fontsize.find('span').text(HB.config.bookread.fontSize);
            fontSizePreview(HB.config.bookread.fontSize);
        }
    }
    function setFontStyle() {
        if (HB.config.bookread.fontStyle) {
            $fontstyle.find('a[data-value=\'' + HB.config.bookread.fontStyle + '\']').trigger('click');
        }
    }
    function setTsukkomiSite() {
        if (HB.config.bookread.tsukkomiSite) {
            $tsukkomiSite.find('a[data-value=\'' + HB.config.bookread.tsukkomiSite + '\']').trigger('click');
        }
    }
    function setMusicPlayer() {
        if (HB.config.bookread.musicPlayer) {
            $musicPlayer.find('a[data-value=\'' + HB.config.bookread.musicPlayer + '\']').trigger('click');
        }
    }
    function CloseReadSideBarBox() {
        $('#J_ReadSideBarBox .active').removeClass('active');
        $('#J_ReadSideBarCnt').hide();
    }
    function themePreview(value) {
        var href = $('#J_readbookStyle').attr('href');
        var newHref = href.replace(/\/\w+\.css/, '/' + value + '.css');
        $('#J_readbookStyle').attr('href', newHref);
    }
    function fontFamilyPreview(value) {
        $('.read-bd').css({
            fontFamily: HB.config.bookread.fonts[value]
        });
    }
    function fontSizePreview(value) {
        $('.read-bd').css({
            fontSize: value + 'px'
        });
    }
    function fontStylePreview() {
        StranBody();
    }
    function tsukkomiPreview(value) {
        $bookcnt.removeAttr('class');
        $bookcnt.addClass(value);
    }
    function musicPlayerPreview(value) {
        value == 0 ? $('.huanyin').show()  : $('.huanyin').hide();
    }
    function setSubmitBtn() {
        var str = $theme.find('.act').attr('data-value') + ',' + $fontfamily.find('.act').attr('data-value') + ',' + $fontsize.find('span').text() + ',' + $fontstyle.find('.act').attr('data-value') + ',' + $tsukkomiSite.find('.act').attr('data-value') + ',' + $musicPlayer.find('.act').attr('data-value');
        $submit.attr('data-cookie', str);
        changeBookImage();
        clearTimeout(resizeId);
        resizeId = setTimeout(changeBookImage, 200);
    }
    setFontFamliy();
    setFontSize();
    setTheme();
    setFontStyle();
    setTsukkomiSite();
    setMusicPlayer();
    setSubmitBtn();
    $('#J_BarrageStart').click(function () {
        var self = $(this);
        if (self.attr('disabled') == 1) return false;
        $.ajax({
            url: HB.config.rootPath + 'chapter/get_tsukkomi_list',
            data: {
                page: 0,
                count: 1000,
                chapter_id: HB.book.chapter_id
            },
            beforeSend: function () {
                var text = self.text();
                self.attr('disabled', 1).attr('data-text', text);
                self.text(text + '中...');
            },
            type: 'GET',
            success: function (res) {
                if (res.code == 100000) {
                    $('#J_BarrageBox').addClass('opened').find('.btn-close').show();
                    res.userinfo = HB.userinfo;
                    barrageInstance = $('#J_Barrage').qfcbarrage(res);
                    if ($(document).scrollTop() >= $(document).height() - $(window).height()) {
                        $('.container').css('margin-bottom', '140px');
                        $(document).scrollTop(parseInt($(document).scrollTop() + 140));
                    } else {
                        $('.container').css('margin-bottom', '140px');
                    }
                } else {
                    HB.util.alert(res.tip);
                }
            },
            complete: function () {
                var text = self.attr('data-text');
                self.attr('disabled', 0);
                self.text(text);
            }
        });
    });
    $('#J_BarrageBox .btn-close').click(function () {
        $('#J_BarrageBox').removeClass('opened').find('.btn-close').hide();
        $('#J_Barrage').empty();
        $('.container').css('margin-bottom', 0);
    });
    $('#submit_barrage').click(function () {
        if (HB.userinfo.reader_id == 0) {
            HB.util.loginDialog();
            return;
        }
        if (!HB.userinfo.tel_num && !HB.userinfo.license) {
            HB.util.identifyDialog(HB.urlinfo.redirect);
            return;
        }
        if (!HB.userinfo.tel_num && HB.userinfo.license && HB.userinfo.redis_license) {
            HB.util.alert('请耐心等待，我们会在12小时内进行实名制备案。谢谢您的配合！', 3);
            return;
        }
        var self = $(this);
        if (self.attr('disabled')) return;
        var text = $.trim($('#text_barrage').val());
        if (text.length == 0) {
            HB.util.alert('弹幕内容为空！', 1);
            return false;
        }
        if (text.length > 50) {
            HB.util.alert('弹幕50字以内！');
            return false;
        }
        if (barrageInstance) {
            $.ajax({
                url: HB.config.rootPath + 'chapter/add_tsukkomi',
                data: {
                    book_id: HB.book.book_id,
                    chapter_id: HB.book.chapter_id,
                    paragraph_index: 1,
                    tsukkomi_content: text
                },
                beforeSend: function () {
                    self.attr('disabled', true);
                },
                complete: function () {
                    self.attr('disabled', false);
                },
                success: function (res) {
                    if (res.code == 100000) {
                        barrageInstance.insert(text);
                        $('#text_barrage').val('');
                        $('#J_TsukkomiNum').text(parseInt($('#J_TsukkomiNum').text()) + 1);
                        var num = $($bookread.find('p').get(0)).find('.J_Num');
                        if (num.text()) {
                            num.text(parseInt(num.text()) + 1);
                        } else {
                            num.addClass('num1');
                            num.text(1);
                        }
                    } else {
                        HB.util.alert(res.tip);
                    }
                }
            });
        }
    });
    + function () {
        var data = {
        };
        var count = 10;
        var page = 0;
        $bookread.find('.chapter').each(function (index) {
            var i = index + 1;
            if (tsukkomi_num_json[i]) {
                $(this).append('<i data-pgid=\'' + i + '\' class=\'J_Num num num1\'>' + tsukkomi_num_json[i] + '</i>');
            } else {
                $(this).append('<i data-pgid=\'' + i + '\' class=\'J_Num num\'></i>');
            }
        });
        $bookcnt.find('.read-hd .chapter').each(function () {
            var i = 0;
            if (tsukkomi_num_json[i]) {
                $(this).append('<i data-pgid=\'' + i + '\' class=\'J_Num num num1\'>' + tsukkomi_num_json[i] + '</i>');
            } else {
                $(this).append('<i data-pgid=\'' + i + '\' class=\'J_Num num\'></i>');
            }
        });
        template.helper('subString', function (str, len, hasDot) {
            var newLength = 0;
            var newStr = '';
            var chineseRegex = /[^\x00-\xff]/g;
            var singleChar = '';
            var strLength = str.replace(chineseRegex, '**').length;
            for (var i = 0; i < strLength; i++) {
                singleChar = str.charAt(i).toString();
                if (singleChar.match(chineseRegex) != null) {
                    newLength += 2;
                } else {
                    newLength++;
                }
                if (newLength > len) {
                    break;
                }
                newStr += singleChar;
            }
            if (hasDot && strLength > len) {
                newStr += '...';
            }
            return newStr;
        });
        function bindTsukkomiEvent() {
            $('#J_BookCnt').on('click', '.J_PagePrev', function () {
                var self = $(this);
                if (self.hasClass('noprev')) return;
                if (self.attr('disabled')) return;
                var obj = self.closest('.J_ChapterCnt');
                page = parseInt(self.attr('data-no'));
                var paragraph_index = parseInt(obj.closest('.chapter-comment-wrap').prev('.chapter').find('.J_Num').attr('data-pgid'));
                if (isNaN(paragraph_index)) {
                    paragraph_index = parseInt(obj.closest('.chapter-comment-wrap').prev('i').attr('data-pgid'));
                }
                $.ajax({
                    url: HB.config.rootPath + 'chapter/get_paragraph_tsukkomi_list',
                    data: {
                        page: page,
                        count: count,
                        chapter_id: HB.book.chapter_id,
                        paragraph_index: paragraph_index
                    },
                    beforeSend: function () {
                        self.attr('disabled', true);
                    },
                    complete: function () {
                        self.attr('disabled', false);
                    },
                    type: 'GET',
                    success: function (res) {
                        if (res.code == 100000) {
                            data = res.data;
                            commentRender(1, obj, res.data, page);
                        } else {
                            HB.util.alert(res.tip, 1);
                        }
                    }
                });
            });
            $('#J_BookCnt').on('click', '.J_PageNext', function () {
                var self = $(this);
                if (self.hasClass('nonext')) return;
                if (self.attr('disabled')) return;
                var obj = self.closest('.J_ChapterCnt');
                page = parseInt(self.attr('data-no'));
                var paragraph_index = parseInt(obj.closest('.chapter-comment-wrap').prev('.chapter').find('.J_Num').attr('data-pgid'));
                if (isNaN(paragraph_index)) {
                    paragraph_index = parseInt(obj.closest('.chapter-comment-wrap').prev('i').attr('data-pgid'));
                }
                $.ajax({
                    url: HB.config.rootPath + 'chapter/get_paragraph_tsukkomi_list',
                    data: {
                        page: page,
                        count: count,
                        chapter_id: HB.book.chapter_id,
                        paragraph_index: paragraph_index
                    },
                    beforeSend: function () {
                        self.attr('disabled', true);
                    },
                    complete: function () {
                        self.attr('disabled', false);
                    },
                    type: 'GET',
                    success: function (res) {
                        data = res.data;
                        if (res.code == 100000) {
                            commentRender(1, obj, res.data, page);
                        } else {
                            HB.util.alert(res.tip, 1);
                        }
                    }
                });
            });
            $bookcnt.on('click', '.J_Num', function () {
                var paragraph_index = $(this).attr('data-pgid');
                var $p = $(this).closest('.chapter');
                var open = true;
                if ($p.next('div.chapter-comment-wrap').length) {
                    open = false;
                }
                $bookcnt.find('div.chapter-comment-wrap').remove();
                if (open) {
                    page = 0;
                    $.ajax({
                        url: HB.config.rootPath + 'chapter/get_paragraph_tsukkomi_list',
                        type: 'GET',
                        data: {
                            page: page,
                            count: count,
                            chapter_id: HB.book.chapter_id,
                            paragraph_index: paragraph_index
                        },
                        success: function (res) {
                            data = res.data;
                            if (res.code == 100000) {
                                commentRender(0, $p, res.data, page);
                            } else {
                                HB.util.alert(res.tip, 1);
                            }
                        }
                    });
                }
            });
            $('#J_BookImage').on('click', '.J_Num', function () {
                var paragraph_index = $(this).attr('data-pgid');
                var open = true;
                var $p = $(this);
                var $next = $p.next('div');
                if ($next.hasClass('chapter-comment-wrap')) {
                    open = false;
                }
                $('#J_ImgRead').find('div.chapter-comment-wrap').remove();
                if (open) {
                    page = 0;
                    $.ajax({
                        url: HB.config.rootPath + 'chapter/get_paragraph_tsukkomi_list',
                        type: 'GET',
                        data: {
                            page: page,
                            count: count,
                            chapter_id: HB.book.chapter_id,
                            paragraph_index: paragraph_index
                        },
                        success: function (res) {
                            data = res.data;
                            if (res.code == 100000) {
                                commentRender(0, $p, res.data, page);
                                $next = $p.next('div');
                                if ($next.hasClass('chapter-comment-wrap')) {
                                    $next.css('top', parseFloat($p.attr('top')) + 18 + 'px');
                                }
                            } else {
                                HB.util.alert(res.tip, 1);
                            }
                        }
                    });
                }
            });
            $bookcnt.on('click', '.J_ChapterSubmit', function () {
                if (HB.userinfo.reader_id == 0) {
                    HB.util.loginDialog();
                    return;
                }
                if (!HB.userinfo.tel_num && !HB.userinfo.license) {
                    HB.util.identifyDialog(HB.urlinfo.redirect);
                    return;
                }
                if (!HB.userinfo.tel_num && HB.userinfo.license && HB.userinfo.redis_license) {
                    HB.util.alert('请耐心等待，我们会在12小时内进行实名制备案。谢谢您的配合！', 3);
                    return;
                }
                var self = $(this);
                if (self.attr('disabled')) return;
                var box = self.closest('.chapter-comment-wrap');
                var p = box.prev('.chapter');
                var paragraph_index = parseInt(p.find('.J_Num').attr('data-pgid'));
                if (isNaN(paragraph_index)) {
                    var num = self.num = box.prev('i');
                } else {
                    var num = self.num = p.find('.J_Num');
                }
                var obj = box.find('.J_ChapterCnt');
                var value = $.trim(self.prev('input').val());
                if (value.length > 50) {
                    HB.util.alert('间贴50字以内！');
                    return false;
                }
                if (value.length = 0) {
                    HB.util.alert('间贴内容为空！', 1);
                    return false;
                }
                $.ajax({
                    url: HB.config.rootPath + 'chapter/add_tsukkomi',
                    data: {
                        book_id: HB.book.book_id,
                        chapter_id: HB.book.chapter_id,
                        paragraph_index: num.attr('data-pgid'),
                        tsukkomi_content: value
                    },
                    beforeSend: function () {
                        self.attr('disabled', true);
                    },
                    complete: function () {
                        self.attr('disabled', false);
                    },
                    success: function (res) {
                        if (res.code == 100000) {
                            data = res.data;
                            newData = {
                                tsukkomi_id: res.data.tsukkomi_info.tsukkomi_id,
                                reader_info: {
                                    reader_id: HB.userinfo.reader_id,
                                    reader_name: HB.userinfo.reader_name,
                                    avatar_thumb_url: HB.userinfo.avatar_thumb_url,
                                    vip_lv: HB.userinfo.vip_lv
                                },
                                is_author: false,
                                ctime: new Date().Format('yyyy-MM-dd hh:mm:ss'),
                                tsukkomi_content: value,
                                like_amount: 0,
                                unlike_amount: 0,
                                is_like: 0,
                                is_unlike: 0
                            };
                            if (HB.book.up_reader_id == HB.userinfo.reader_id) {
                                newData.is_author = true;
                            }
                            commentRender(1, obj, data, page);
                            if (barrageInstance) {
                                barrageInstance.insert(value);
                            }
                            self.prev('input').val('');
                            $('#J_TsukkomiNum').text(parseInt($('#J_TsukkomiNum').text()) + 1);
                            if (self.num.text()) {
                                self.num.text(parseInt(self.num.text()) + 1);
                            } else {
                                self.num.addClass('num1');
                                self.num.text(1);
                            }
                        } else {
                            HB.util.alert(res.tip, 1);
                        }
                    }
                });
            });
        }
        bindTsukkomiEvent();
        function commentRender(typeNo, obj, data, page) {
            var type = [
                'template-chapter-comment',
                'template-chapter-comment-list'
            ];
            var pageInfo = {
                page: page,
                pageNo: Math.ceil(data.paragraph_tsukkomi_amount / count)
            };
            if (data.paragraph_tsukkomi_amount >= count) {
                pageInfo.show = true;
            }
            if (page != 0) {
                pageInfo.prev = true;
            }
            if (page + 1 < pageInfo.pageNo) {
                pageInfo.next = true;
            }
            data.pageInfo = pageInfo;
            for (var i = 0; i < data.tsukkomi_list.length; i++) {
                if (HB.book.up_reader_id == data.tsukkomi_list[i].reader_info.reader_id) {
                    data.tsukkomi_list[i].is_author = true;
                }
            }
            var html = template(type[typeNo], data);
            if (typeNo === 0) {
                obj.after(html);
            } else if (typeNo === 1) {
                obj.empty().append(html);
            }
        }
    }();
    + function () {
        $('#J_BtnGuide').click(function () {
            HB.util.Cookie.set('qdgd', '1', {
                domain: document.domain,
                path: '/',
                expires: 365
            });
            var a = '<div style="position:fixed; top:50%; left:50%; margin-left:-265px; margin-top:-143px; z-index:100;" class="guideBox"><a href="javascript:" class="redBtn">关闭提醒</a></div><div style="position:fixed;z-index:99; width:100%; height:100%; display:; top:0; left:0;" class="guideMask"></div>';
            $('.guideMask,.guideBox').remove();
            $('body').append(a);
            $('.guideBox,.guideMask').show();
            $('.guideBox a.redBtn').click(function () {
                $('.guideMask,.guideBox').remove();
            });
        });
        '1' != HB.util.Cookie.get('qdgd') && $('#J_BtnGuide').click();
    }();
    (function () {
        $(document).on('keydown', function (event) {
            if (event.target.nodeName.toLowerCase() == 'input') return;
            37 == event.keyCode ? prevPage()  : 39 == event.keyCode && nextPage();
        });
        function prevPage() {
            window.location.href = $('#J_BtnPagePrev').attr('href');
        }
        function nextPage() {
            window.location.href = $('#J_BtnPageNext').attr('href');
        }
    }) ();
});
