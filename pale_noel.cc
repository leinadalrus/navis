//
// modified by David on: 2024-13-01
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
  float magnitude;
};

struct ShadowmapTuple
{
  float x, y, w;
  // NOTE(Tuple): a tuple is a point if `w` is 1;
  // [...]a tuple is a vector when `w` is 0;
};

struct ShadowGeometry
{
  Vector2 vertices[4];
};

struct LightInfo
{
  Vector2 position;
  Rectangle bounds;

  float outer_radius;
  int shadow_index;
};

struct Actor
{
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

class DragCommand : IComponentCommand
{
  IComponentCommand* drag_command;
  EDragDropEvents drag_drop_events;
  struct Magnitude magnitude;

public:
  void Execute(Actor& actor) override
  {
  }

  int Drag()
  {
    if (this->drag_command)
      return 1;
    if (this->drag_drop_events == EDragDropEvents::OnDrop)
      return 1;

    if (this->magnitude.magnitude >= 1.0f)
      return 1;

    return 0;
  }
};

class DropCommand : IComponentCommand
{
  IComponentCommand* drop_command;

public:
  void Execute(Actor& actor) override
  {
  }

  int Drop()
  {
    if (this->drop_command != nullptr)
      return 1;
    return 0;
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

  IComponentCommand &drag_command;
  IComponentCommand &drop_command;

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
      drag_command.Execute(actor);
    case EDragDropEvents::OnDrop:
      drop_command.Execute(actor);
    default:
      break;
    }

    return comp;
  }
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
  void Reify_Tuple(
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
  int Conevision_Culling()
  {
    LightInfo light_info{};
    // shadowmap collision bounds goes down to 0 is-equal-to culled

    std::array<float, 1> w = { 0.0f };// : Angular frequency
    std::time_t t = std::time(nullptr);

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
public:

  void init_volume(VolumetricTuple& volumetric_tuple)
  {
    this->volume = volumetric_tuple;
  }

  int reify_volume(VolumetricTuple& volumetric_tuple) const
  {
    if (volumetric_tuple.dimensions.width > this->volume.dimensions.width
        && volumetric_tuple.dimensions.height > this->volume.dimensions.height)
      return 1;

    return 0;
  }
};

// NOTE(Mediator):
class BuoyantForceProxy
{
  VolumetricTuple volume;

public:
  void init_buoyancy(VolumetricTuple& volumetric_tuple)
  {
    this->volume = volumetric_tuple;
  }

  int reify_buoyancy() const
  {
    if (this->volume.position.x > 0.0f
        && this->volume.position.y > 0.0f)
      return 1;

    return 0;
  }
};

// NOTE(Memento):
class VolumetricEfficiencyTable
{
};

int main()
{
  std::printf("Window Application: Pale Noel.");

  auto shadowmap_composer = new ShadowmapComposer();
  struct ShadowmapTuple shadowmap_tuple;
  std::array<struct LightInfo, 1> shadowmap_indices = {};

  InitWindow(440, 320, "Pale Noel");

  SetTargetFPS(24);

  while (!WindowShouldClose())
  {
    BeginDrawing();

    ClearBackground(BLANK);

    for (const auto& shadow : shadowmap_indices)
      shadowmap_composer->Update_Light(shadow.shadow_index,
          shadowmap_tuple.x,
          shadowmap_tuple.y);

    EndDrawing();
  }

  CloseWindow();
}
