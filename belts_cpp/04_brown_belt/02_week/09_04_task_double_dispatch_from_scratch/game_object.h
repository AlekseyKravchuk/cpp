#pragma once

// файл "game_object.h" содержит интерфейс GameObject и объявление функции Collide

class Unit;
class Building;
class Tower;
class Fence;

// абстрактный базовый (родительский) класс для классов-потомков Unit, Building, Tower и Fence
struct GameObject {
  virtual ~GameObject() = default;

  virtual bool Collide(const GameObject& that) const = 0;

  virtual bool CollideWith(const Unit& that) const = 0;
  virtual bool CollideWith(const Building& that) const = 0;
  virtual bool CollideWith(const Tower& that) const = 0;
  virtual bool CollideWith(const Fence& that) const = 0;
};

bool Collide(const GameObject& first, const GameObject& second);
