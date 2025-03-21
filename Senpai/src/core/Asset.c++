#include <core/scene/Asset.h++>

namespace Senpai {
   u32 Asset::get_asset_id() const {
      return this->id;
   }
   u32 Asset::set_asset_id(u32 id) {
      this->id = id;
      return id;
   }

   void AssetRegistry::clear() {
      assets.clear();
      assetTypeId_to_assetIds.clear();
      name_to_assetId.clear();
   }
}