/****************************************************************************
* *
** Copyright (C) 2016 Michael Yang
** Contact: ohmyarchlinux@gmail.com
**
** This file is part of the examples of the feed_parser.
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of the feed_parser library nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/

#include <feed/parser.h>

int main(int argc, char *argv[]) {
    feed::parser parser;
    const auto feed = parser.parse("https://ipn.li/kernelpanic/feed");
    if (!feed) {
        std::cerr << "Failed." << std::endl;

        return 1;
    }

    std::cout << "channel:" << std::endl
              << "  title: " << feed->title() << std::endl
              << "  link: " << feed->link() << std::endl
              << "  description: " << feed->description() << std::endl
              << "  items:" << std::endl;

    for (const auto &item : feed->items()) {
        std::cout << "    item:" << std::endl;
        auto title = item.title();
        if (title)
            std::cout << "      title:" << title.value() << std::endl;
        auto link = item.link();
        if (link)
            std::cout << "      link:" << link.value() << std::endl;
        // auto description = item.description();
        // if (description)
        // std::cout << "      description:" << description.value()
        //            << std::endl;
        auto author = item.author();
        if (title)
            std::cout << "      author:" << author.value() << std::endl;
    }
}
