/****************************************************************************
**
** Copyright (C) 2016 Michael Yang
** Contact: ohmyarchlinux@gmail.com
**
** This file is part of the feed_parser.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include <boost/property_tree/xml_parser.hpp>
#include <cpprest/details/basic_types.h>
#include <feed/parser.h>

namespace feed {
boost::optional<data> parser::parse(const std::string &uri) {
    web::http::client::http_client client(
        utility::conversions::to_string_t(uri), http_client_config_);
    const auto response = client.request(web::http::methods::GET);

    boost::property_tree::ptree root;

    data data;

    try {
        utility::istringstream_t stream(
            response.get().extract_string(true).get());

        boost::property_tree::read_xml(stream, root);

        const auto &rss_node = root.get_child("rss");

        const auto &channel_node = rss_node.get_child("channel");
        data.title_ = channel_node.get<std::string>("title");
        data.link_ = channel_node.get<std::string>("link");
        data.description_ = channel_node.get<std::string>("description");

        const auto &image_node = channel_node.get_child_optional("image");
        if (image_node) {
            image image;
            image.url_ = image_node->get<std::string>("url");
            image.title_ = image_node->get<std::string>("title");
            image.link_ = image_node->get<std::string>("link");
            image.width_ = image_node->get_optional<std::uint16_t>("width");
            image.height_ = image_node->get_optional<std::uint16_t>("height");
            image.description_ =
                image_node->get_optional<std::string>("description");

            data.image_ = std::move(image);
        }

        for (const auto &child : channel_node)
            if (child.first == "item") {
                item item;
                const auto &item_node = child.second;
                item.title_ = item_node.get_optional<std::string>("title");
                item.link_ = item_node.get_optional<std::string>("link");
                item.description_ =
                    item_node.get_optional<std::string>("description");
                item.author_ = item_node.get_optional<std::string>("author");

                const auto &enclosure_node =
                    item_node.get_child("enclosure.<xmlattr>");
                item.enclosure_ = enclosure(
                    enclosure_node.get<std::string>("url"),
                    enclosure_node.get_optional<std::uint64_t>("length"),
                    enclosure_node.get<std::string>("type"));

                data.items_.emplace_back(std::move(item));
            }

        return std::move(data);
    } catch (const web::http::http_exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return {};
}

bool parser::set_proxy(const std::string &uri) {
    try {
        http_client_config_.set_proxy(
            web::web_proxy(utility::conversions::to_string_t(uri)));

        return true;
    } catch (const web::uri_exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return false;
}
}
