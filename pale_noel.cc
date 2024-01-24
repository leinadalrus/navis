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

enum class CellCorners
{
  IOTA = 0b0000,
  BOTTOM_LEFT_CORNER = 0b0001,
  BOTTOM_RIGHT_CORNER = 0b0010,
  BOTTOM_CORNERS = 0b0011,
  TOP_RIGHT_CORNER = 0b0100,
  POSITIVE_DIAGONAL_ANGLE = 0b0101,
  RIGHT_FACE_VERTICES = 0b0110,
  POSITIVE_RIGHT_ANGLE_VERTICES = 0b0111,
  TOP_LEFT_CORNER = 0b1000,
  LEFT_FACE_VERTICES = 0b1001,
  NEGATIVE_DIAGONAL_ANGLE = 0b1010,
  NEGATIVE_RIGHT_ANGLE_VERTICES = 0b1011,
  NORTH_FACE_ENCOMPASSING = 0b1100,
  NORTH_WEST_ENCOMPASSING = 0b1101,
  NORTH_EAST_ENCOMPASSING = 0b1110,
  ALL_FACES_ENCOMPASSING = 0b1111
};

struct Magnitude
{
  float magnitude;
};

struct CollisionTuple
{
  float x, y;
  // NOTE(Tuple): a tuple is a point if `w` is 1;
  // [...]a tuple is a vector when `w` is 0;
};

struct CollisionGeometry
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

struct VolumetricTuple
{
  Rectangle dimensions;
  Vector2 position;
};

class IComponentCommand
{
public:
  ~IComponentCommand() = default;

  virtual void Execute() = 0;
  // NOTE(virtual): `virtual` keyword helps with non-static member(s)-
  // -dynamic dispatch
};

class DragCommand : IComponentCommand
{
  IComponentCommand* drag_command;
  EDragDropEvents drag_drop_events;
  struct Magnitude magnitude;

public:
  void Execute() override
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
  void Execute() override
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

  IComponentCommand& drag_command;
  IComponentCommand& drop_command;

public:
  void Execute() override
  {
    IInputHandler::Handle();
  }

  void Handle()
  {
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
      drag_command.Execute();
    case EDragDropEvents::OnDrop:
      drop_command.Execute();
    default:
      break;
    }
  }
};

// NOTE(Marching Square): Planar [Gridmap]/Tuple
class MarchingTuple
{
public:
  Vector2 vertex; // : Vertex Point
};

class AStar
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

// NOTE(shadows): a-la Inverted Collision
class CollisionComposer
{
  // Collections:
  std::array<CollisionTuple, 8> collision_tuples;
  std::array<LightInfo, 8> light_infos;

public:
  int ConeVision_Culling()
  {
    LightInfo light_info{};
    // collision bounds goes down to 0 is-equal-to culled

    std::array<float, 1> w = { 0.0f };// : Angular frequency
    std::time_t t = std::time(nullptr);

    for (auto shadow_index : collision_tuples)
    {
      // Collision tuples need folding maps : functional
      Vector2 starting_amp = (Vector2){ shadow_index.x, shadow_index.y },
          ending_amp = (Vector2){ shadow_index.x, shadow_index.y };

      float x = starting_amp.x
          * std::sin(w[t - light_info.outer_radius])
          + light_info.outer_radius;

      float reversed_x = ending_amp.y
          * std::cos(w[t - light_info.outer_radius])
          + light_info.outer_radius;

      AStar::Reconstruct_Path((int)x, (int)reversed_x);
      // once each individual ray collides another, cull the stack at FIFO
    }
    // NOTE(do-while): a do-while loop is declared here for single looping

    return 0;
  }// NOTE(collision): for each light index cast against collision-
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
  BuoyantForceFlyweight buoyant_flyweight{};
  BuoyantForceProxy forces_proxy{};

public:
  VolumetricEfficiencyTable(
      BuoyantForceFlyweight flyweight,
      BuoyantForceProxy proxy
  )
  {
    this->buoyant_flyweight = flyweight;
    this->forces_proxy = proxy;
  }

  ~VolumetricEfficiencyTable() = default;

  static int Update_VEff_Table(VolumetricEfficiencyTable* eff_table)
  {
    // NOTE(tuples): observe VolumetricTuple[s]-
    // -against Buoyancy Forces
    return 0;
  }

  static int Read_VEff_Table(VolumetricEfficiencyTable* eff_table)
  {
    return 0;
  }
};

class VolumetricEfficiencyComputer
{
  struct VolumetricTuple volume;
  struct CollisionTuple shadow;

public:
  void Reify_Tuple(
      struct VolumetricTuple _volume,
      struct CollisionTuple _shadow)
  {
    this->volume = _volume;
    this->shadow = _shadow;

    std::printf("Volumetric := \t%p", &this->volume);
    std::printf("Collision := \t%p", &this->shadow);
  }
}; // Volumetric Efficiency data-structures must be seperate processes

// Main
int main()
{
  std::printf("Window Application: Pale Noel.");

  auto collision_composer = new CollisionComposer();
  struct CollisionTuple collision_tuple{};
  std::array<struct LightInfo, 1> collision_indices = {};

  BuoyantForceFlyweight buoyant_flyweight{};
  BuoyantForceProxy buoyant_proxy{};

  InitWindow(440, 320, "Pale Noel");

  SetTargetFPS(24);

  while (!WindowShouldClose())
  {
    BeginDrawing();

    ClearBackground(BLANK);

    auto* eff_table = new VolumetricEfficiencyTable(
        buoyant_flyweight,
        buoyant_proxy
    );

    for (auto& shadow : collision_indices)
      collision_composer->Update_Light(shadow.shadow_index,
          collision_tuple.x,
          collision_tuple.y);

    VolumetricEfficiencyTable::Read_VEff_Table(eff_table);
    VolumetricEfficiencyTable::Update_VEff_Table(eff_table);

    EndDrawing();
  }

  CloseWindow();
}
