#include "items/weapons.hpp"

// #TODO: eventually add a full implementation
c_hook_call<0x00B5FD0C> weapon_get_age_hook0({ .pointer = weapon_get_age });
c_hook_call<0x00B5FD49> weapon_get_age_hook1({ .pointer = weapon_get_age });
c_hook_call<0x00B61B20> weapon_get_age_hook2({ .pointer = weapon_get_age });
c_hook_call<0x00B61B68> weapon_get_age_hook3({ .pointer = weapon_get_age });
c_hook_call<0x00B61F4B> weapon_get_age_hook4({ .pointer = weapon_get_age });
c_hook_call<0x00B62005> weapon_get_age_hook5({ .pointer = weapon_get_age });
c_hook_call<0x00B6948B> weapon_get_age_hook6({ .pointer = weapon_get_age });

// #TODO: eventually add a full implementation
c_hook_call<0x00B5BE0A> weapon_has_infinite_ammo_hook00({ .pointer = weapon_has_infinite_ammo });
c_hook_call<0x00B5D03D> weapon_has_infinite_ammo_hook01({ .pointer = weapon_has_infinite_ammo });
c_hook_call<0x00B61253> weapon_has_infinite_ammo_hook02({ .pointer = weapon_has_infinite_ammo });
c_hook_call<0x00B615DB> weapon_has_infinite_ammo_hook03({ .pointer = weapon_has_infinite_ammo });
c_hook_call<0x00B62AC9> weapon_has_infinite_ammo_hook04({ .pointer = weapon_has_infinite_ammo });
c_hook_call<0x00B63F67> weapon_has_infinite_ammo_hook05({ .pointer = weapon_has_infinite_ammo });
c_hook_call<0x00B643BA> weapon_has_infinite_ammo_hook06({ .pointer = weapon_has_infinite_ammo });
c_hook_call<0x00B649B6> weapon_has_infinite_ammo_hook07({ .pointer = weapon_has_infinite_ammo });
c_hook_call<0x00B64CB6> weapon_has_infinite_ammo_hook08({ .pointer = weapon_has_infinite_ammo });
c_hook_call<0x00B65EB2> weapon_has_infinite_ammo_hook09({ .pointer = weapon_has_infinite_ammo });
c_hook_call<0x00B678D2> weapon_has_infinite_ammo_hook10({ .pointer = weapon_has_infinite_ammo });
c_hook_call<0x00B68614> weapon_has_infinite_ammo_hook11({ .pointer = weapon_has_infinite_ammo });