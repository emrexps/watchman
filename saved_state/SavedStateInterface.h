/* Copyright 2017-present Facebook, Inc.
 * Licensed under the Apache License, Version 2.0 */
#pragma once

#include "thirdparty/jansson/jansson.h"
#include "watchman_string.h"

namespace watchman {

class SCM;

// An interface that returns information about saved states associated with
// specific source control commits. Clients using scm-aware queries can
// receive information about the most recent known good saved state when the
// mergebase changes, along with the changed files since that saved state. The
// client can then update the current state based on the saved state and the
// modified files since that state's commit, rather than processing all
// changes since the prior mergebase.
class SavedStateInterface {
 public:
  virtual ~SavedStateInterface();

  // Returns an appropriate SavedStateInterface implementation for the
  // specified storage type. Returns a managed pointer to the saved state
  // interface if successful. Throws if the storage type is not recognized.
  static std::unique_ptr<SavedStateInterface> getInterface(
      w_string_piece storageType);

  // The commit ID of a saved state and a JSON blob of information clients can
  // use to access the saved state.  The contents of the info varies with the
  // storage type.
  struct SavedStateResult {
    w_string commitId;
    json_ref savedStateInfo;
  };
  // Returns saved state information for the most recent commit prior to and
  // including lookupCommitId that has a valid saved state for the specified
  // storage key. The contents of the storage key and the return value vary with
  // the storage type.
  virtual SavedStateResult getMostRecentSavedState(
      w_string_piece lookupCommitId) const = 0;
};
} // namespace watchman
