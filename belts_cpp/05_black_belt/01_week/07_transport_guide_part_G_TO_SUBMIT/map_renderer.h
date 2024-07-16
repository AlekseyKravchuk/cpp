#pragma once

#include "json.h"
#include "svg.h"
#include "sphere.h"
#include "descriptions.h"

namespace SvgMap {
    struct Stop {
        std::string name;
        Svg::Point position;
    };

    struct Bus {
        std::string name;
        std::vector<std::shared_ptr<Stop>> stops;
        Svg::Color color = Svg::NoneColor;
    };
}

class MapRenderer {
public:
    MapRenderer(const Descriptions::StopsDict& stops_dict,
        const Descriptions::BusesDict& buses_dict,
        const Json::Dict& render_settings_json);

    std::string Render() const;

private:
    struct RenderSettings {
        double width;
        double height;
        double padding;
        double stop_radius;
        double line_width;
        uint32_t stop_label_font_size;
        Svg::Point stop_label_offset;
        Svg::Color underlayer_color;
        double underlayer_width;
        std::vector<Svg::Color> color_palette;
    };

    static RenderSettings MakeRenderSettings(const Json::Dict& json);
    static Svg::Color ConvertJsonColorToSvg(const Json::Node&);

    struct MinMaxGeoCoords {
        double min_lat;
        double min_lon;
        double max_lat;
        double max_lon;
    };
    
    MinMaxGeoCoords GetMinMaxGeoCoords(const Descriptions::StopsDict& stops_dict) const;
    double GetZoomCoef(MinMaxGeoCoords geo_coords) const;

    std::map<std::string, std::shared_ptr<SvgMap::Stop>> stops_;
    std::map<std::string, std::shared_ptr<SvgMap::Bus>> buses_;
    RenderSettings render_settings_;
};
