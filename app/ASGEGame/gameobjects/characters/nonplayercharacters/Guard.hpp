// Created by Ben on 12/04/2022

// References
#include "../../mapstuff/alarm/AlarmManager.hpp"

// Base class
#include "../NonPlayerCharacter.hpp"

class Guard : public NonPlayerCharacter
{
 public:
  Guard()           = default;
  ~Guard() override = default;

  // Init guard stuff
  void start() override;
  // Detect if a player is in line of sight
  void update(const ASGE::GameTime& game_time) override;

  // execution order ):
  void setAlarmManager(const std::shared_ptr<AlarmManager>& _alarm_manger);

 private:
  // Collision enter, used for attacking
  void onCollisionEnter(bcw::Collider* collision) override;
  // Collision exit, used for stopping attacking
  void onCollisionExit(bcw::Collider* collision) override;


  // Once a player has been spotted, count down from LOCK_ON_TIME for lock on
  void countDownToLockOn(double delta_time);
  // Once the target is out of range, count down from LOCK_OFF_TIME for lock off
  void countDownToLockOff(double delta_time);
  // Check if a player or the target is in line of sight
  bool castRayAndCheckForPlayer(bool set_locked_onto_player);
  // Check if the player is range
  bool checkIfTargetIsInRange();

  // Attack the player!
  void attack();

  // Contains a reference to all the alarms in the level, so they can be set off
  std::shared_ptr<AlarmManager> alarm_manager = nullptr;
  const float ALARM_DURATION                  = 10.0F; // How long the alarms will be set off for

  // The guards' behavior changing variables
  const double LOCK_ON_TIME = 0.0; // How long the guard  will take to lock onto a player
  double lock_on_timer      = 0.0;
  bool attempting_lock_on   = false;

  const double LOCK_OFF_TIME = 1.0;   // How long the guard will take to lock off a player
  double lock_off_timer      = 0.0;   // doubling up for verboseness
  bool attempting_lock_off   = false; // ^

  const double ATTACK_COOL_DOWN = 2.0F; // How long between attacks
  double attack_timer           = 0.0;

  // How much damage the guard will do
  const int DAMAGE = 25;

  // How close the player has to be to the guard for the guard to continue chasing
  const float CHASE_RANGE = 500.0F;
  // How close to the player the guard has to be to see them
  const float VIEW_RANGE  = 4000.0F;

  bool is_attacking = false;
};
