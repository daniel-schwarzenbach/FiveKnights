#pragma once

#include "./Base.h++"
#include "./container/Vector.h++"

namespace Senpai{
   struct Asset {
      private:
         u32 id = (-1);
      public:
         String name;
         u32 get_asset_id() const;
         // should only be called by AssetRegistry
         u32 set_asset_id(u32 id);
         inline virtual ~Asset() = default;
   };

   template<typename T>
   concept AssetType = std::is_base_of_v<Asset, T>;

   template<AssetType AssetT>
   constexpr u32 asset_type_id() {
      static const u32 id = get_subtype_id<Asset>();
      return id;
   }

   // where all Asset types live
   struct AssetRegistry {
      // sored assets by asset id
      Vector<UniquePtr<Asset>> assets;
      // generate unique asset id
      u32 generate_unique_id();
      // keep track of type Ids
      Map<u32,Set<u32>> assetTypeId_to_assetIds;
      // keep track of names
      Map<String,u32> name_to_assetId;

      // non templated functions
      void clear();

      // templated functions
      template<AssetType AssetT>
      void add_asset(AssetT&& asset){
         u32 assetId = assets.size();
         asset.set_asset_id(assetId);
         
         if(name_to_assetId.contains(asset.name)){
            debug_log("Asset with that name already exists!");
            return;
         }
         name_to_assetId[asset.name] = assetId;
         assetTypeId_to_assetIds[asset_type_id<AssetT>()].insert(assetId);
         assets.push_back(make_unique<Asset>(move(asset)));
      }

      template<AssetType AssetT>
      AssetT& get_asset(u32 assetId){
         if(assets.size() <= assetId){
            debug_log("Asset does not exist!");
            throw std::out_of_range("Asset does not exist!");
         }
         // begin debug
         #ifndef NDEBUG
            // check if asset has the correct type
            if(assetTypeId_to_assetIds[asset_type_id<AssetT>()].contains(assetId)){
               debug_log("Asset has not the correct type!");
               throw std::runtime_error("Asset has not the correct type!");
            }
         #endif
         // end debug
         return *static_cast<Ptr<AssetT>>(assets[assetId].get());
      }

      template<AssetType AssetT>
      AssetT& get_asset(String name){
         if(name_to_assetId.contains(name)){
            return get_asset<AssetT>(name_to_assetId[name]);
         } else {
            debug_log("Asset does not exist!");
            throw std::out_of_range("Asset does not exist!");
         }
      }

      template<AssetType AssetT>
      u32 get_asset_id(String assetName){
         if(name_to_assetId.contains(assetName)){
            return name_to_assetId[assetName];
         } else {
            debug_log("Asset does not exist!");
            return u32(-1);
         }
      }
   };
}