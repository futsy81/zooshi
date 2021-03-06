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

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include <cassert>
#include "fplbase/renderer.h"

namespace fpl {

namespace zooshi {

class StateNode {
 public:
  virtual ~StateNode() {}

  virtual void AdvanceFrame(int delta_time, int* next_state) = 0;
  virtual void RenderPrep() {}
  virtual void Render(fplbase::Renderer* render) = 0;
  virtual void HandleUI(fplbase::Renderer* /*renderer*/) {}
  virtual void OnEnter(int /*previous_state*/) {}
  virtual void OnExit(int /*next_state*/) {}
};

template <int state_count_>
class StateMachine {
 public:
  typedef int StateId;

  // Initializes the StateMachine. You must call SetCurrentStateId to a valid
  // state before running AdvanceFrame or Render.
  StateMachine() : current_state_id_(-1) {}

  StateNode* get_state(StateId state_id) { return &states_[state_id]; }

  // Associate an enum value with a specific node.
  void AssignState(StateId id, StateNode* state) {
    assert(valid_id(id));
    states_[id] = state;
  }

  // Run the logic on the current game state.
  void AdvanceFrame(int delta_time) {
    if (valid_id(current_state_id_)) {
      StateId new_id = current_state_id_;
      states_[current_state_id_]->AdvanceFrame(delta_time, &new_id);
      SetCurrentStateId(new_id);
    }
  }

  // Prepare to render the current game state.
  void RenderPrep() {
    if (valid_id(current_state_id_)) {
      states_[current_state_id_]->RenderPrep();
    }
  }
  // Render the current game state.
  void Render(fplbase::Renderer* renderer) {
    if (valid_id(current_state_id_)) {
      states_[current_state_id_]->Render(renderer);
    }
  }

  // Prepare to render the current game state.
  void HandleUI(fplbase::Renderer* renderer) {
    if (valid_id(current_state_id_)) {
      states_[current_state_id_]->HandleUI(renderer);
    }
  }

  // Transitions to the new state, calling the appropriate OnEnter and OnExit
  // functions.
  void SetCurrentStateId(StateId new_id) {
    if (new_id != current_state_id_) {
      if (valid_id(current_state_id_)) {
        states_[current_state_id_]->OnExit(new_id);
      }
      if (valid_id(new_id)) {
        states_[new_id]->OnEnter(current_state_id_);
      }
      current_state_id_ = new_id;
    }
  }

  StateId current_state_id() { return current_state_id_; }

  // The state machine reaches a terminal state when it's state is less than 0
  // or greater than the number of declared states (i.e. state_count_)
  bool done() { return !valid_id(current_state_id_); }

 private:
  bool valid_id(StateId id) { return id >= 0 && id < state_count_; }

  StateId current_state_id_;
  StateNode* states_[state_count_];
};

}  // zooshi
}  // fpl

#endif  // STATE_MACHINE_H_
