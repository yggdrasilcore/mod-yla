#include "YLADBCRegistry.h"

std::vector<DBCDefinition> dbcRegistry = {
    REGISTER_DBC(GemProperties, GemPropertiesEntry, sGemPropertiesStore),
    REGISTER_DBC(Spell,         SpellEntry,         sSpellStore),
    REGISTER_DBC(Talent,        TalentEntry,        sTalentStore),
    REGISTER_DBC(TalentTab,     TalentTabEntry,     sTalentTabStore),
};
