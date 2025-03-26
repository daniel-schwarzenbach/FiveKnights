#include <Senpai>


namespace Senpai {

bool RenderComponent::operator<(const RenderComponent &other) const {
   return z > other.z;
}

bool UIComponent::operator<(const UIComponent &other) const {
   return z > other.z;
}

} // namespace Senpai