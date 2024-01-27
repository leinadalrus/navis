#define _CRT_NONSTDC_NO_DEPRECATE
#define strdup _strdup

// Math:
#include <cmath>
#include <ctime>

// Collection:
#include <array>
#include <vector>

#include "../include/pale_noel.h"
#include "../ext/minunit.h"

enum class EDragDropEvents
{
  DragEnter,
  DragLeave,
  OnDrag,
  OnDrop
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

struct CollisionGeometry
{
  Vector2 vertices[4];
};

// i.e Marching Info
struct CollisionInfo
{
  Vector2 position;
  Rectangle bounds;

  float outer_radius;
  int collision_index;
};

struct VolumeMagnitude
{
  Rectangle dimensions;
};

// NOTE(Marching Square): Planar [Gridmap]/Tuple
struct MarchingTuple
{
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
  std::array<struct MarchingTuple, 8> square_tuples;
  std::array<struct CollisionInfo, 8> collision_infos;

public:
  int ConeVision_Culling()
  {
    struct CollisionInfo collision_info{};
    // collision bounds goes down to 0 is-equal-to culled

    std::array<float, 1> w = { 0.0f };// : Angular frequency
    std::time_t t = std::time(nullptr);

    for (auto volume_index : square_tuples)
    {
      // Collision tuples need folding maps : functional
      Vector2 starting_amp = (Vector2){
        volume_index.vertex.x,
          volume_index.vertex.y},
          ending_amp = (Vector2){
            volume_index.vertex.x,
              volume_index.vertex.y};

      float x = starting_amp.x
          * std::sin(w[t - collision_info.outer_radius])
          + collision_info.outer_radius;

      float reversed_x = ending_amp.y
          * std::cos(w[t - collision_info.outer_radius])
          + collision_info.outer_radius;

      AStar::Reconstruct_Path((int)x, (int)reversed_x);
      // once each individual ray collides another, cull the stack at FIFO
    }
    // NOTE(do-while): a do-while loop is declared here for single looping

    return 0;
  }// NOTE(collision): for each collision index cast against collision-
  // -cast a shadow within the collision index's rectangle collision bounds

  void Update_Light(int index, float x, float y)
  {
    collision_infos[index].position.x = x;
    collision_infos[index].position.y = y;

    collision_infos[index].bounds.x = x - collision_infos[index].outer_radius;
    collision_infos[index].bounds.y = y - collision_infos[index].outer_radius;
  }
};

// NOTE(Buoyant Force): faking buoyant force with Marching data
class BuoyantForceFlyweight
{
  MarchingTuple square;
  VolumeMagnitude volume;
public:

  void init_volume(MarchingTuple& square_tuple)
  {
    this->square = square_tuple;
  }

  int reify_volume(CollisionInfo& volumetric_tuple) const
  {
    if (volumetric_tuple.bounds.width > this->volume.dimensions.width
        && volumetric_tuple.bounds.height > this->volume.dimensions.height)
      return 1;

    return 0;
  }
};

// NOTE(Mediator):
class BuoyantForceProxy
{
  MarchingTuple volume;

public:
  void init_buoyancy(MarchingTuple& volumetric_tuple)
  {
    this->volume = volumetric_tuple;
  }

  int reify_buoyancy() const
  {
    if (this->volume.vertex.x > 0.0f
        && this->volume.vertex.y > 0.0f)
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
    // NOTE(tuples): observe MarchingTuple[s]-
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
  struct MarchingTuple volume;
  struct MarchingTuple square;

public:
  void Reify_Tuple(
      struct MarchingTuple _volume,
      struct MarchingTuple _square)
  {
    this->volume = _volume;
    this->square = _square;

    std::printf("Volume := \t%p", &this->volume);
    std::printf("Collision := \t%p", &this->square);
  }
}; // Marching Efficiency data-structures must be seperate processes

int tests_run = 0;

static char *test_given_param_data()
{
  int destination = 0, source = 0;
  mu_assert(_strdup("Assume Destination isn't Source"), destination != source);
  return 0;
}

static char *test_units()
{
  mu_run_test(test_given_param_data);
  return 0;
}

// Main
int main()
{
  std::printf("Window Application: Pale Noel.");

  char *res = test_units();

  if (res != 0)
    std::printf("[FAILED]:\t%s\n", res);
  else
    std::printf("[PASSED]:\t%s\n", res);

  auto collision_composer = new CollisionComposer();
  struct MarchingTuple square_tuple{};
  std::array<struct CollisionInfo, 1> collision_indices = {};

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

    for (auto& index : collision_indices)
      collision_composer->Update_Light(index.collision_index,
          square_tuple.vertex.x,
          square_tuple.vertex.y);

    VolumetricEfficiencyTable::Read_VEff_Table(eff_table);
    VolumetricEfficiencyTable::Update_VEff_Table(eff_table);

    EndDrawing();
  }

  CloseWindow();

  return res == 0;
}

