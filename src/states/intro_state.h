// Copyright 2015 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ZOOSHI_INTRO_STATE_H_
#define ZOOSHI_INTRO_STATE_H_

#include "camera.h"
#include "config_generated.h"
#include "flatui/font_manager.h"
#include "fplbase/asset_manager.h"
#include "fplbase/input.h"
#include "fplbase/utilities.h"
#include "pindrop/pindrop.h"
#include "states/state_machine.h"
#include "states/states.h"

namespace fpl {
namespace zooshi {

struct InputConfig;
struct World;
class FullScreenFader;

static const int kIntroStateFadeTransitionDuration = 2000;

class IntroState : public StateNode {
public:
  IntroState();
  virtual ~IntroState() {}
  void Initialize(fplbase::InputSystem* input_system, World* world,
                  const Config* config, FullScreenFader* fader,
                  pindrop::AudioEngine* audio_engine);
  virtual void AdvanceFrame(int delta_time, int* next_state);
  virtual void RenderPrep();
  virtual void Render(fplbase::Renderer* renderer);
  virtual void OnEnter(int previous_state);
  virtual void OnExit(int next_state);

protected:
  // Shows or hides the "intro to cardboard" box that spawns when you start
  // the game in cardboard mode.
  void SetBoxVisibility(bool visibility);

  // The world so that we can get the player.
  World* world_;

  // The input system so that we can get input.
  fplbase::InputSystem* input_system_;

  // Fade the screen.
  FullScreenFader* fader_;

  // Time after fade triggered and before fade started. In milliseconds.
  // Also takes values kFadeTimerPending before being triggered,
  // and kFadeTimerComplete after completing.
  int fade_timer_;

  // We need a pointer to the master bus to fade the audio when we enter this
  // state.
  pindrop::Bus master_bus_;

  Camera main_camera_;
#if FPLBASE_ANDROID_VR
  Camera cardboard_camera_;
#endif  // FPLBASE_ANDROID_VR
};

}  // zooshi
}  // fpl

#endif  // ZOOSHI_INTRO_STATE_H_
