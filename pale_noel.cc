//
// TODO: Refactor
//

#include "include/pale_noel.h"

// Math:
#include <cmath>
#include <ctime>

// Collection:
#include <array>
#include <vector>

enum class EDragDropEvents
{
  DragEnter,
  DragLeave,
  OnDrag,
  OnDrop
};

enum class EMaterialCondition
{
  Active,
  Dirty,
  Standby
};

struct Magnitude
{
};

struct ShadowmapTuple
{
  float x, y, z, w;
  // NOTE(Tuple): a tuple is a point if `w` is 1;
  // [...]a tuple is a vector when `w` is 0;
};

struct ShadowGeometry
{
  Vector2 vertices[4];
};

struct LightInfo
{
  EMaterialCondition light_state;
  Vector2 position;

  RenderTexture texture_mask;
  Rectangle bounds;

  float outer_radius;
  int shadow_index;
};

struct Position
{
  Vector2 position;
};

struct Name
{
  std::string name;
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

struct Actor
{
};

struct PlayerBundle
{
  struct Name name;    // ID
  struct Actor actor;// Instantiation

  // Properties:
  struct Collision dimension{
  };
  struct Position position{
  };
};

struct VolumetricTuple
{
  Rectangle dimensions;
  Vector2 position;
};

class IComponentCommand
{
public:
  ~IComponentCommand() = default;

  virtual void Execute(Actor& actor) = 0;
  // NOTE(virtual): `virtual` keyword helps with non-static member(s)-
  // -dynamic dispatch
};

class CursorClickDragCommand : IComponentCommand
{
  IComponentCommand* click_drag_command;
  EDragDropEvents drag_drop_events;
  struct Magnitude magnitude;

public:
  void Execute(Actor& actor) override
  {
  }
};

class CursorDropCommand : IComponentCommand
{
  IComponentCommand* drop_command;

public:
  void Execute(Actor& actor) override
  {
  }
};

template<typename T>
class IInputHandler : public IComponentCommand
{
  decltype(auto) rvalue_cast(T&& t)
  {
    using ResultType = std::remove_reference<T>&&;
    return static_cast<ResultType>(t);
  }

  IComponentCommand* drag_command{};
  IComponentCommand* drop_command{};

  IComponentCommand* attack_input{};
  IComponentCommand* delay_input{};
  IComponentCommand* sustain_input{};
  IComponentCommand* release_input{};

public:
  void Execute(Actor& actor) override
  {
    IInputHandler::Handle();
  }

  int Handle(Actor& actor)
  {
    auto comp = [this](T t)
    {
      this->rvalue_cast(t, t);
    };

    EDragDropEvents drag_drop_event;
    EMaterialCondition material_condition;

    switch (material_condition)
    {
    case EMaterialCondition::Active:
    case EMaterialCondition::Dirty:
    case EMaterialCondition::Standby:
    default:
      break;
    }

    switch (drag_drop_event)
    {
    case EDragDropEvents::DragEnter:
    case EDragDropEvents::DragLeave:
    case EDragDropEvents::OnDrag:
      drag_command->Execute(actor);
    case EDragDropEvents::OnDrop:
      drop_command->Execute(actor);
    default:
      break;
    }

    return comp;
  }

  int is_input_active(int event)
  {
    return event;
  }
  // NOTE(input handler):
  // create a strategy-command pattern for this...
};

class AStarPathfinder
{
public:
  static int Reconstruct_Path(int from, int current)
  {
    std::vector<int> paths_vec = { current };

    for (const auto& path : paths_vec)
    {
      if (from < current)
        paths_vec.push_back(path);

      return path;
    }

    return 0;
  }
};

class TabletopComputer
{
  struct VolumetricTuple volume;
  struct ShadowmapTuple shadow;

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
    this->scalar_tuple_sentinel(this->volume, this->shadow);

    return c;
  }

  void scalar_tuple_sentinel(
      struct VolumetricTuple _volume,
      struct ShadowmapTuple _shadow)
  {
    this->volume = _volume;
    this->shadow = _shadow;

    std::printf("Volumetric := \t%p", &this->volume);
    std::printf("Shadow := \t%p", &this->shadow);
  }
};

// NOTE(shadows): a-la Inverted Lightmap
class ShadowmapComposer
{
  // Data:
  ShadowmapTuple light_shadow_tuple;
  ShadowGeometry shadow_geometry;

  // Collections:
  std::array<ShadowmapTuple, 8> shadowmap_tuples;
  std::array<LightInfo, 8> light_infos;

public:
  int W_Tuple(ShadowmapTuple tuple_a, ShadowmapTuple tuple_b)
  {
    LightInfo light_info{};
    // shadowmap collision bounds goes down to 0 is-equal-to culled

    std::array<float, 1> w = { 0.0f };// : Angular frequency
    std::time_t t = std::time(nullptr);

    auto pathfinder = new AStarPathfinder();

    for (auto shadow_index : shadowmap_tuples)
    {
      // Shadowmap tuples need folding maps : functional
      Vector2 starting_amp = (Vector2){ shadow_index.x, shadow_index.y },
          ending_amp = (Vector2){ shadow_index.x, shadow_index.y };

      float x = starting_amp.x
          * std::sin(w[t - light_info.outer_radius])
          + light_info.outer_radius;

      float reversed_x = ending_amp.y
          * std::cos(w[t - light_info.outer_radius])
          + light_info.outer_radius;

      AStarPathfinder::Reconstruct_Path((int)x, (int)reversed_x);
      // once each individual ray collides another, cull the stack at FIFO
    }
    // NOTE(do-while): a do-while loop is declared here for single looping

    return 0;
  }// NOTE(shadowmap): for each light index cast against collision-
  // -cast a shadow within the light index's rectangle collision bounds

  void Update_Light(int index, float x, float y)
  {
    light_infos[index].position.x = x;
    light_infos[index].position.y = y;

    light_infos[index].bounds.x = x - light_infos[index].outer_radius;
    light_infos[index].bounds.y = y - light_infos[index].outer_radius;
  }
};

// NOTE(Buoyant Force): faking buoyant force with Volumetric data
class BuoyantForceFlyweight
{
  VolumetricTuple volume;
};

// NOTE(Mediator):
class BuoyantForceProxy
{
  VolumetricTuple volume;
};

// NOTE(Memento):
class VolumetricEfficiencyTable
{
};

int main()
{
  std::printf("Window Application: Pale Noel.");

  auto shadowmap_composer = new ShadowmapComposer();
  struct ShadowmapTuple active_tuple{}, dirty_tuple{}, standby_tuple{};
  std::array<struct LightInfo, 1> shadowmap_indices = {};

  InitWindow(440, 320, "Pale Noel");

  SetTargetFPS(24);

  while (!WindowShouldClose())
  {
    BeginDrawing();

    ClearBackground(BLANK);

    shadowmap_composer->W_Tuple(active_tuple, dirty_tuple);

    for (const auto& shadow : shadowmap_indices)
      shadowmap_composer->Update_Light(shadow.shadow_index,
          active_tuple.x,
          active_tuple.y);

    EndDrawing();
  }

  CloseWindow();
}
