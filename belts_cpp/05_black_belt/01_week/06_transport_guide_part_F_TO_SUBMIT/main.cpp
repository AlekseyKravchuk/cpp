#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

using namespace std;

namespace Svg {
	struct Point {
		double x = 0.0, y = 0.0;
	};

	struct Rgb {
		uint8_t red = 0, green = 0, blue = 0;
	};

	using Color = std::variant<std::monostate, std::string, Rgb>;
	const Color NoneColor{};

	void RenderColor(std::ostream& out, std::monostate);
	void RenderColor(std::ostream& out, const std::string&);
	void RenderColor(std::ostream& out, Rgb);
	void RenderColor(std::ostream& out, const Color&);

	class Object {
	public:
		virtual void Render(std::ostream& out) const = 0;
		virtual ~Object() = default;
	};
	using ObjectPtr = std::unique_ptr<Object>;

	template <typename Owner>
	class PathProps {
	public:
		Owner& SetFillColor(const Color& color) {
			fill = color;
			return AsOwner();
		}
		Owner& SetStrokeColor(const Color& color) {
			stroke = color;
			return AsOwner();
		}
		Owner& SetStrokeWidth(double width) {
			strokeWidth = width;
			return AsOwner();
		}
		Owner& SetStrokeLineCap(const std::string& lineCap) {
			strokeLineCap = lineCap;
			return AsOwner();
		}
		Owner& SetStrokeLineJoin(const std::string& lineJoin) {
			strokeLineJoin = lineJoin;
			return AsOwner();
		}
		void RenderAttrs(std::ostream& out) const {
			out << " fill=\"";
			RenderColor(out, fill);
			out << "\" stroke=\"";
			RenderColor(out, stroke);
			out << "\" stroke-width=\"" << strokeWidth << '\"';
			if (strokeLineCap) {
				out << " stroke-linecap=\"" << *strokeLineCap << '\"';
			}
			if (strokeLineJoin) {
				out << " stroke-linejoin=\"" << *strokeLineJoin << '\"';
			}
		}

	private:
		Color fill = NoneColor;
		Color stroke = NoneColor;
		double strokeWidth = 1.0;
		std::optional<std::string> strokeLineCap;
		std::optional<std::string> strokeLineJoin;

		Owner& AsOwner() {
			return static_cast<Owner&>(*this);
		}
	};

	class Circle : public Object, public PathProps<Circle> {
	public:
		Circle& SetCenter(Point);
		Circle& SetRadius(double);
		void Render(std::ostream&) const override;

	private:
		double cx = 0.0;
		double cy = 0.0;
		double r = 1.0;
	};

	class Polyline : public Object, public PathProps<Polyline> {
	public:
		Polyline& AddPoint(Point);
		void Render(std::ostream&) const override;
	private:
		std::vector<Point> points;
	};

	class Text : public Object, public PathProps<Text> {
	public:
		Text& SetPoint(Point);
		Text& SetOffset(Point);
		Text& SetFontSize(uint32_t);
		Text& SetFontFamily(const std::string&);
		Text& SetData(const std::string&);
		void Render(std::ostream&) const override;
	private:
		double x = 0.0;
		double y = 0.0;
		double dx = 0.0;
		double dy = 0.0;
		uint32_t fontSize = 1;
		std::optional<std::string> fontFamily;
		std::string text;
	};

	class Document : public Object {
	public:
		template <typename ObjectType>
		void Add(ObjectType obj) {
			objects.push_back(make_unique<ObjectType>(std::move(obj)));
		}

		void Render(std::ostream& out) const override;
	private:
		std::vector<ObjectPtr> objects;
	};
}

namespace Svg {
	void RenderColor(ostream& out, monostate) {
		out << "none";
	}
	void RenderColor(ostream& out, const string& value) {
		out << value;
	}
	void RenderColor(ostream& out, Rgb rgb) {
		out << "rgb("
			<< static_cast<int>(rgb.red) << ','
			<< static_cast<int>(rgb.green) << ','
			<< static_cast<int>(rgb.blue) << ')';
	}
	void RenderColor(ostream& out, const Color& color) {
		visit([&out](const auto& value) {
			RenderColor(out, value);
		}, color);
	}

	Circle& Circle::SetCenter(Point p) {
		cx = p.x;
		cy = p.y;
		return *this;
	}
	Circle& Circle::SetRadius(double new_r) {
		r = new_r;
		return *this;
	}
	void Circle::Render(ostream& out) const {
		out << "<circle cx=\"" << cx << "\" cy=\"" << cy << "\" r=\"" << r << '\"';
		PathProps::RenderAttrs(out);
		out << " />";
	}

	Polyline& Polyline::AddPoint(Point p) {
		points.push_back(move(p));
		return *this;
	}
	void Polyline::Render(ostream& out) const {
		out << "<polyline points=\"";
		for (const Point& p : points) {
			out << p.x << ',' << p.y << " ";
		}
		out << '\"';
		PathProps::RenderAttrs(out);
		out << " />";
	}

	Text& Text::SetPoint(Point p) {
		x = p.x;
		y = p.y;
		return *this;
	}
	Text& Text::SetOffset(Point p) {
		dx = p.x;
		dy = p.y;
		return *this;
	}
	Text& Text::SetFontSize(uint32_t fz) {
		fontSize = fz;
		return *this;
	}
	Text& Text::SetFontFamily(const string& ff) {
		fontFamily = ff;
		return *this;
	}
	Text& Text::SetData(const string& s) {
		text = s;
		return *this;
	}
	void Text::Render(ostream& out) const {
		out << "<text x=\"" << x << "\" y=\"" << y << "\" dx=\"" << dx << "\" dy=\"" << dy << '\"'
			<< " font-size=\"" << fontSize << '\"';
		if (fontFamily) {
			out << " font-family=\"" << *fontFamily << '\"';
		}
		PathProps::RenderAttrs(out);
		out << " >" << text << "</text>";
	}

	void Document::Render(ostream& out) const {
		out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
			<< "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">";
		for (const auto& obj : objects) {
			obj->Render(out);
		}
		out << "</svg>";
	}
}

int main() {
    Svg::Document svg;

    svg.Add(
        Svg::Polyline{}
        .SetStrokeColor(Svg::Rgb{ 140, 198, 63 })  // soft green
        .SetStrokeWidth(16)
        .SetStrokeLineCap("round")
        .AddPoint({ 50, 50 })
        .AddPoint({ 250, 250 })
    );

    for (const auto point : { Svg::Point{50, 50}, Svg::Point{250, 250} }) {
        svg.Add(
            Svg::Circle{}
            .SetFillColor("white")
            .SetRadius(6)
            .SetCenter(point)
        );
    }

    svg.Add(
        Svg::Text{}
        .SetPoint({ 50, 50 })
        .SetOffset({ 10, -10 })
        .SetFontSize(20)
        .SetFontFamily("Verdana")
        .SetFillColor("black")
        .SetData("C")
    );
    svg.Add(
        Svg::Text{}
        .SetPoint({ 250, 250 })
        .SetOffset({ 10, -10 })
        .SetFontSize(20)
        .SetFontFamily("Verdana")
        .SetFillColor("black")
        .SetData("C++")
    );

    svg.Render(std::cout);

	return 0;
}
