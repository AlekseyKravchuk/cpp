#include "Common.h"

#include <memory>   // std::shared_ptr
#include <utility>  // std::pair

using namespace std;

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`

class Shape : public IShape {
  public:
    virtual void SetPosition(Point position) override {
        _position = position;
    }

    virtual Point GetPosition() const override {
        return _position;
    }

    virtual void SetSize(Size size) override {
        _size = size;
    }

    virtual Size GetSize() const override {
        return _size;
    }

    virtual void SetTexture(std::shared_ptr<ITexture> sh_ptr) override {
        _texture = std::move(sh_ptr);
    }

    virtual ITexture* GetTexture() const override {
        return _texture.get();
    }

    /*Image: std::vector<std::string>*/
    virtual void Draw(Image& img) const override {
        struct Border {
            Point top_left;
            Point lower_right;
        };

        Border img_border {
            _position,
            {_position.x + _size.width, _position.y + _size.height}
        };

        Size texture_size = _texture
                            ? _texture->GetSize()
                            : Size{0, 0};

        Border texture_border {
            _position,
            {_position.x + texture_size.width, _position.y + texture_size.height}
        };

        for (int y = img_border.top_left.y; y < img_border.lower_right.y && y < img.size(); ++y) {
            for (int x = img_border.top_left.x; x < img_border.lower_right.x && x < img[y].size(); ++x) {
                if (Contains({x, y})) {
                    auto filling = y < texture_border.lower_right.y && x < texture_border.lower_right.x
                            ? _texture->GetImage()[y - texture_border.top_left.y][x - texture_border.top_left.x]
                            : '.';
                    img[y][x] = filling;
                }
            }
        }
    }

  protected:
    Point _position;                 // _position: {x, y}
    Size _size;                      // _size: {width, height}
    shared_ptr<ITexture> _texture;

    virtual bool Contains(Point p) const = 0;

    explicit Shape() = default;

    explicit Shape(Point position,
                   Size size,
                   shared_ptr<ITexture> texture)
            : _position(position),
              _size(size),
              _texture(std::move(texture)) {}
};

class Rectangle : public Shape {
  public:
    explicit Rectangle() = default;

    explicit Rectangle(Point position, Size size, shared_ptr<ITexture> texture)
            : Shape(position, size, std::move(texture)) {}

    virtual std::unique_ptr<IShape> Clone() const override {
        return make_unique<Rectangle>(_position, _size, _texture);
    }

  private:
    virtual bool Contains(Point p) const override {
        return p.y >= _position.y &&
               p.y < _position.y + _size.height &&
               p.x >= _position.x &&
               p.x < _position.x + _size.width;
    }
};

class Ellipse : public Shape {
  public:
    explicit Ellipse() = default;

    Ellipse(Point position, Size size, shared_ptr<ITexture> texture)
            : Shape(position, size, std::move(texture)) {}

    virtual std::unique_ptr<IShape> Clone() const override {
        return make_unique<Ellipse>(_position, _size, _texture);
    }

  private:
    virtual bool Contains(Point p) const override {
        return IsPointInEllipse(
                Point{p.x - _position.x, p.y - _position.y},
                _size);
    }
};

// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
    switch (shape_type) {
        case ShapeType::Rectangle:
            return make_unique<Rectangle>();
        case ShapeType::Ellipse:
            return make_unique<Ellipse>();
        default:
            return nullptr;
    }
}