#pragma once
#include <string>
#include <json.hpp>

using json = nlohmann::json;

struct championStats {
	float abilityHaste;
	float abilityPower;
	float armor;
	float armorPenetrationFlat;
	float armorPenetrationPercent;
	float attackDamage;
	float attackRange;
	float attackSpeed;
	float bonusArmorPenetrationPercent;
	float bonusMagicPenetrationPercent;
	float critChance;
	float critDamage;
	float currentHealth;
	float healShieldPower;
	float healthRegenRate;
	float lifeSteal;
	float magicLethality;
	float magicPenetrationFlat;
	float magicPenetrationPercent;
	float magicResist;
	float maxHealth;
	float moveSpeed;
	float omnivamp;
	float physicalLethality;
	float physicalVamp;
	float resourceMax;
	float resourceRegenRate;
	std::string resourceType;
	float resourceValue;
	float spellVamp;
	float tenacity;
};

static void from_json(const json& j, championStats& cs) {
	j.at("abilityHaste").get_to(cs.abilityHaste);
	j.at("abilityPower").get_to(cs.abilityPower);
	j.at("armor").get_to(cs.armor);
	j.at("armorPenetrationFlat").get_to(cs.armorPenetrationFlat);
	j.at("armorPenetrationPercent").get_to(cs.armorPenetrationPercent);
	j.at("attackDamage").get_to(cs.attackDamage);
	j.at("attackRange").get_to(cs.attackRange);
	j.at("attackSpeed").get_to(cs.attackSpeed);
	j.at("bonusArmorPenetrationPercent").get_to(cs.bonusArmorPenetrationPercent);
	j.at("bonusMagicPenetrationPercent").get_to(cs.bonusMagicPenetrationPercent);
	//j.at("cooldownReduction").get_to(cs.cooldownReduction);
	j.at("critChance").get_to(cs.critChance);
	j.at("critDamage").get_to(cs.critDamage);
	j.at("currentHealth").get_to(cs.currentHealth);
	j.at("healShieldPower").get_to(cs.healShieldPower);
	j.at("healthRegenRate").get_to(cs.healthRegenRate);
	j.at("lifeSteal").get_to(cs.lifeSteal);
	j.at("magicLethality").get_to(cs.magicLethality);
	j.at("magicPenetrationFlat").get_to(cs.magicPenetrationFlat);
	j.at("magicPenetrationPercent").get_to(cs.magicPenetrationPercent);
	j.at("magicResist").get_to(cs.magicResist);
	j.at("maxHealth").get_to(cs.maxHealth);
	j.at("moveSpeed").get_to(cs.moveSpeed);
	j.at("omnivamp").get_to(cs.omnivamp);
	j.at("physicalLethality").get_to(cs.physicalLethality);
	j.at("physicalVamp").get_to(cs.physicalVamp);
	j.at("resourceMax").get_to(cs.resourceMax);
	j.at("resourceRegenRate").get_to(cs.resourceRegenRate);
	j.at("resourceType").get_to(cs.resourceType);
	j.at("resourceValue").get_to(cs.resourceValue);
	j.at("spellVamp").get_to(cs.spellVamp);
	j.at("tenacity").get_to(cs.tenacity);
}
