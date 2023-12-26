#include "include/pale-noel.hh"

enum class DragDropEvents
{
  DragEnter,
  DragLeave,
  OnDrag,
  OnDrop
};

struct Magnitude
{
};

struct Vector3FTuple
{
  float x, y, z, w;
  // NOTE(Tuple): a tuple is a point if `w` is 1;
  // [...]a tuple is a vector when `w` is 0;
};

struct Position
{
  Vector2 position;
};

struct Name
{
  std::string name;
};

struct Entity
{
};

struct Collision
{
  Rectangle dimensions;
};

struct Card
{
  Rectangle dimensions;
  Vector2 position;
};

struct TabletopScalar
{
  Rectangle dimensions;
  Vector2 position;
};

struct PlayerBundle
{
  struct Name name;     // ID
  struct Entity entity; // Instantiation

  // Properties:
  struct Collision dimension{};
  struct Position position{};
};

class ICompositeCommand
{
public:
  virtual void Execute()
  {
  }
  // NOTE(virtual): `virtual` keyword helps with non-static member(s)-
  // -dynamic dispatch
};

template<typename T>
class IInputHandler : public ICompositeCommand
{
private:
  decltype(auto) rvalue_cast(T&& a, T&& b)
  {
    auto c = a + b;
    using ResultType = std::remove_reference<T>&&;
    return static_cast<ResultType>(a);
  }

public:
  void Execute() override
  {
    IInputHandler::Handle();
  }

  int Handle()
  {
    auto comp = [this](T t)
    {
      this->rvalue_cast(t, t);
    };

    return comp;
  }
};

class TabletopComputer
{
  struct TabletopScalar scalar;
  struct Vector3FTuple tuple;

public:
  template<typename T>
  decltype(auto) tuple_arbitrary_operand(T x, T y, T z, T w)
  {
    auto lambda = [](T&& x, T&& y, T&& z, T&& w)
    {
      return (x, y, z, w);
    };
    // NOTE(double ampersand): we have inferred an R-Value declaration.
    // Note: this is different from a `declaration of an R-Value inference.`

    auto c = lambda.template operator()<T>(x, y, z, w);
    this->scalar_tuple_sentinel(scalar, tuple);

    return c;
  }

  void scalar_tuple_sentinel(
      struct TabletopScalar _scalar,
      struct Vector3FTuple _tuple
  )
  {
    this->scalar = _scalar;
    this->tuple = _tuple;

    std::printf("Scalar := \t%v", &this->scalar);
    std::printf("Tuple := \t%v", &this->tuple);
  }
};

class CursorClickDragCommand : ICompositeCommand
{
  ICompositeCommand click_drag_command;
  DragDropEvents drag_drop_events;
  struct Magnitude magnitude;

public:
  void Execute() override
  {
  }
};

class CursorDropCommand : ICompositeCommand
{
  ICompositeCommand drop_command;

public:
  void Execute() override
  {
  }
};

int main()
{
  std::printf("Window Application: Pale Noel.");

  InitWindow(440, 320, "Pale Noel");

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(BLANK);
    EndDrawing();
  }

  CloseWindow();
}
