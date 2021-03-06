/*
 * Copyright 2016 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Author: yeputons@google.com (Egor Suvorov)

#include "pagespeed/kernel/cache/in_memory_cache.h"

#include <map>
#include <utility>
#include "pagespeed/kernel/base/shared_string.h"
#include "pagespeed/kernel/base/string.h"

namespace net_instaweb {

InMemoryCache::InMemoryCache()
    : is_shut_down_(false) {
}

void InMemoryCache::Get(const GoogleString& key, Callback* callback) {
  if (is_shut_down_) {
    ValidateAndReportResult(key, kNotFound, callback);
    return;
  }

  const StringStringMap::const_iterator &value_it = cache_.find(key);
  if (value_it == cache_.end()) {
    ValidateAndReportResult(key, kNotFound, callback);
  } else {
    // SharedString(GoogleString) ctor is explicit, it also creates a new
    // detached buffer for GoogleString which is exactly what we need here.
    *callback->value() = SharedString(value_it->second);
    ValidateAndReportResult(key, kAvailable, callback);
  }
}

void InMemoryCache::Put(const GoogleString& key, SharedString* new_value) {
  if (is_shut_down_) {
    return;
  }

  cache_[key] = new_value->Value().as_string();
}

void InMemoryCache::Delete(const GoogleString& key) {
  if (is_shut_down_) {
    return;
  }

  cache_.erase(key);
}

}  // namespace net_instaweb
