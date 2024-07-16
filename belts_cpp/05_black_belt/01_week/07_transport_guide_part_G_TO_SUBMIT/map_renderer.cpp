#include "map_renderer.h"
#include "utils.h"

#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

MapRenderer::MapRenderer(const Descriptions::StopsDict& stops_dict,
    const Descriptions::BusesDict& buses_dict,
    const Json::Dict& render_settings_json)
    : render_settings_(MakeRenderSettings(render_settings_json))
{
    auto geo_coords = GetMinMaxGeoCoords(stops_dict);
    double zoom_coef = GetZoomCoef(geo_coords);
    for (auto [stop_name, stop_ptr] : stops_dict) {
        const auto [lat, lon] = stop_ptr->position;
        stops_[stop_name] = make_shared<SvgMap::Stop>(SvgMap::Stop{
            stop_name,
            Svg::Point{
                .x = (lon - geo_coords.min_lon)* zoom_coef + render_settings_.padding,
                .y = (geo_coords.max_lat - lat)* zoom_coef + render_settings_.padding,
            },
        });
    }

    for (auto [bus_name, bus_ptr] : buses_dict) {
        vector<shared_ptr<SvgMap::Stop>> bus_stops;
        bus_stops.reserve(bus_ptr->stops.size());
        for (string stop_name : bus_ptr->stops) {
            bus_stops.push_back(stops_[stop_name]);
        }
        buses_[bus_name] = make_shared<SvgMap::Bus>(SvgMap::Bus{
            .name = bus_name,
            .stops = bus_stops,
        });
    }

    int color_counter = 0, colors_count = render_settings_.color_palette.size();
    for (const auto& [_, bus_ptr] : buses_) {
        bus_ptr->color = render_settings_.color_palette[color_counter % colors_count];
        ++color_counter;
    }
}

MapRenderer::RenderSettings MapRenderer::MakeRenderSettings(const Json::Dict& json) {
    vector<Json::Node> stop_label_offset_json = json.at("stop_label_offset").AsArray();

    vector<Json::Node> color_palette_json = json.at("color_palette").AsArray();
    vector<Svg::Color> color_palette;
    color_palette.resize(color_palette_json.size());
    transform(color_palette_json.begin(), color_palette_json.end(), color_palette.begin(), [](const Json::Node& node) {
        return ConvertJsonColorToSvg(node);
    });

    return {
        .width = json.at("width").AsDouble(),
        .height = json.at("height").AsDouble(),
        .padding = json.at("padding").AsDouble(),
        .stop_radius = json.at("stop_radius").AsDouble(),
        .line_width = json.at("line_width").AsDouble(),
        .stop_label_font_size = static_cast<uint32_t>(json.at("stop_label_font_size").AsInt()),
        .stop_label_offset = Svg::Point{ stop_label_offset_json.at(0).AsDouble(), stop_label_offset_json.at(1).AsDouble() },
        .underlayer_color = ConvertJsonColorToSvg(json.at("underlayer_color")),
        .underlayer_width = json.at("underlayer_width").AsDouble(),
        .color_palette = color_palette,
    };
}

Svg::Color MapRenderer::ConvertJsonColorToSvg(const Json::Node& json) {
    if (holds_alternative<vector<Json::Node>>(json.GetBase())) {
        vector<Json::Node> vals = json.AsArray();
        if (vals.size() == 3) {
            return Svg::Rgb{
                .red = static_cast<uint8_t>(vals.at(0).AsInt()),
                .green = static_cast<uint8_t>(vals.at(1).AsInt()),
                .blue = static_cast<uint8_t>(vals.at(2).AsInt()),
            };
        }
        else {
            return Svg::Rgba{
                .red = static_cast<uint8_t>(vals.at(0).AsInt()),
                .green = static_cast<uint8_t>(vals.at(1).AsInt()),
                .blue = static_cast<uint8_t>(vals.at(2).AsInt()),
                .alpha = vals.at(3).AsDouble(),
            };
        }
    }
    else {
        return json.AsString();
    }
}

MapRenderer::MinMaxGeoCoords MapRenderer::GetMinMaxGeoCoords(const Descriptions::StopsDict& stops_dict) const {
    bool first = false;
    double min_lat, max_lat, min_lon, max_lon;
    for (auto [_, stop_ptr] : stops_dict) {
        const auto [lat, lon] = stop_ptr->position;
        if (!first) {
            first = true;
            min_lat = max_lat = lat;
            min_lon = max_lon = lon;
        }
        else {
            min_lat = min(min_lat, lat);
            max_lat = max(max_lat, lat);
            min_lon = min(min_lon, lon);
            max_lon = max(max_lon, lon);
        }
    }

    return { min_lat, min_lon, max_lat, max_lon };
}

double MapRenderer::GetZoomCoef(MapRenderer::MinMaxGeoCoords geo_coords) const {
    auto [min_lat, min_lon, max_lat, max_lon] = geo_coords;

    double eps = 1e-9;
    double zoom_coef = 0;
    if ((abs(max_lon - min_lon) > eps) && (abs(max_lat - min_lat) > eps)) {
        zoom_coef = min(
            (render_settings_.width - 2 * render_settings_.padding) / (max_lon - min_lon),
            (render_settings_.height - 2 * render_settings_.padding) / (max_lat - min_lat)
        );
    }
    else if (abs(max_lon - min_lon) > eps) {
        zoom_coef = (render_settings_.width - 2 * render_settings_.padding) / (max_lon - min_lon);
    }
    else if (abs(max_lat - min_lat) > eps) {
        zoom_coef = (render_settings_.height - 2 * render_settings_.padding) / (max_lat - min_lat);
    }

    return zoom_coef;
}

std::string MapRenderer::Render() const {
    Svg::Document svg;

    for (auto [_, bus_ptr] : buses_) {
        Svg::Polyline polyline;
        for (auto stop_ptr : bus_ptr->stops) {
            auto [x, y] = stop_ptr->position;
            polyline.AddPoint({ x,y });
        }
        svg.Add(
            polyline
            .SetStrokeColor(bus_ptr->color)
            .SetStrokeWidth(render_settings_.line_width)
            .SetStrokeLineCap("round")
            .SetStrokeLineJoin("round")
        );
    }

    for (auto [_, stop_ptr] : stops_) {
        svg.Add(
            Svg::Circle{}
            .SetCenter(stop_ptr->position)
            .SetRadius(render_settings_.stop_radius)
            .SetFillColor("white")
        );
    }

    for (auto [_, stop_ptr] : stops_) {
        // подложка
        svg.Add(
            Svg::Text{}
            .SetPoint(stop_ptr->position)
            .SetOffset(render_settings_.stop_label_offset)
            .SetFontSize(render_settings_.stop_label_font_size)
            .SetFontFamily("Verdana")
            .SetData(stop_ptr->name)
            .SetFillColor(render_settings_.underlayer_color)
            .SetStrokeColor(render_settings_.underlayer_color)
            .SetStrokeWidth(render_settings_.underlayer_width)
            .SetStrokeLineCap("round")
            .SetStrokeLineJoin("round")
        );
        // надпись
        svg.Add(
            Svg::Text{}
            .SetPoint(stop_ptr->position)
            .SetOffset(render_settings_.stop_label_offset)
            .SetFontSize(render_settings_.stop_label_font_size)
            .SetFontFamily("Verdana")
            .SetData(stop_ptr->name)
            .SetFillColor("black")
        );
    }

    stringstream os;
    svg.Render(os);
    return GetQuotedString(os.str());
}
