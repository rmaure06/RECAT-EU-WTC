#include <windows.h>
#include <string>
#include <map>
#include <algorithm>
#include "../lib/include/EuroScopePlugIn.h"

#pragma comment(lib, "../lib/EuroScopePlugInDll.lib")

// Define the Tag Item IDs
const int TAG_ITEM_RECAT = 1;
const int TAG_ITEM_RECAT_SLASH = 2; // New Item ID

class CRecatPlugin : public EuroScopePlugIn::CPlugIn
{
private:
    std::map<std::string, std::string> m_RecatMap;

public:
    CRecatPlugin() : CPlugIn(
        EuroScopePlugIn::COMPATIBILITY_CODE,
        "RECAT-EU WTC", // Plugin Name
        "1.0",        // Version updated
        "Raphaël 1461589",       // Developer
        "GPL v3"  // License
    )
    {
        InitializeRecatDatabase();

        // Register both tag items
        RegisterTagItemType("RECAT WTC", TAG_ITEM_RECAT);
        RegisterTagItemType("RECAT WTC with slash", TAG_ITEM_RECAT_SLASH);
    }

    virtual ~CRecatPlugin()
    {
    }

    void OnGetTagItem(EuroScopePlugIn::CFlightPlan FlightPlan,
        EuroScopePlugIn::CRadarTarget RadarTarget,
        int ItemCode,
        int TagData,
        char sItemString[16],
        int* pColorCode,
        COLORREF* pRGB,
        double* pFontSize) override
    {
        // Only handle our specific tag items
        if (ItemCode != TAG_ITEM_RECAT && ItemCode != TAG_ITEM_RECAT_SLASH)
            return;

        std::string category = "?";

        if (FlightPlan.IsValid()) {
            std::string acType = FlightPlan.GetFlightPlanData().GetAircraftFPType();
            std::transform(acType.begin(), acType.end(), acType.begin(), ::toupper);

            auto it = m_RecatMap.find(acType);
            if (it != m_RecatMap.end()) {
                category = it->second;
            }
        }

        if (ItemCode == TAG_ITEM_RECAT_SLASH) {
            sprintf_s(sItemString, 16, "/%s", category.c_str());
        }
        else {
            strcpy_s(sItemString, 16, category.c_str());
        }

        // --- COLORING ENFORCEMENT ---

        // 1. Tell EuroScope we are defining a custom RGB color
        *pColorCode = EuroScopePlugIn::TAG_COLOR_RGB_DEFINED;

        // 2. Set the specific Orange color from your screenshot (R=255, G=174, B=23)
        *pRGB = RGB(255, 174, 23);
    }

private:
    void InitializeRecatDatabase()
    {
        // ==========================================
        // RECAT-EU WAKE TURBULENCE CATEGORIES
        // ==========================================

        // --- CAT A: SUPER HEAVY ---
        m_RecatMap["A388"] = "A"; // Airbus A380-800
        m_RecatMap["A225"] = "A"; // Antonov An-225

        // --- CAT B: UPPER HEAVY ---
        // Airbus
        m_RecatMap["A359"] = "B"; m_RecatMap["A35K"] = "B"; // A350-900/1000
        m_RecatMap["A345"] = "B"; m_RecatMap["A346"] = "B"; // A340-500/600
        m_RecatMap["A339"] = "B"; // A330-900neo (Often classified as Upper Heavy due to MTOW/Span)
        // Boeing
        m_RecatMap["B741"] = "B"; m_RecatMap["B742"] = "B"; m_RecatMap["B743"] = "B";
        m_RecatMap["B744"] = "B"; m_RecatMap["B748"] = "B"; // B747-400/8
        m_RecatMap["B74S"] = "B"; // B747SP
        m_RecatMap["BLCF"] = "B"; // Dreamlifter
        m_RecatMap["B772"] = "B"; m_RecatMap["B773"] = "B"; m_RecatMap["B77L"] = "B";
        m_RecatMap["B77W"] = "B"; m_RecatMap["B77F"] = "B"; // B777 Family
        m_RecatMap["B778"] = "B"; m_RecatMap["B779"] = "B"; // B777X
        m_RecatMap["B788"] = "B"; m_RecatMap["B789"] = "B"; m_RecatMap["B78X"] = "B"; // Dreamliner
        // Military
        m_RecatMap["C5M"] = "B"; // C-5 Galaxy
        m_RecatMap["A124"] = "B"; // An-124 Ruslan

        // --- CAT C: LOWER HEAVY ---
        // Airbus
        m_RecatMap["A332"] = "C"; m_RecatMap["A333"] = "C"; m_RecatMap["A338"] = "C"; // A330 Classics
        m_RecatMap["A342"] = "C"; m_RecatMap["A343"] = "C"; // A340-200/300
        m_RecatMap["A306"] = "C"; m_RecatMap["A30B"] = "C"; m_RecatMap["A310"] = "C"; // A300/310
        m_RecatMap["A3ST"] = "C"; m_RecatMap["A337"] = "C"; // Beluga / Beluga XL
        m_RecatMap["A400"] = "C"; // A400M
        // Boeing
        m_RecatMap["B762"] = "C"; m_RecatMap["B763"] = "C"; m_RecatMap["B764"] = "C"; // B767
        m_RecatMap["KC46"] = "C"; m_RecatMap["E767"] = "C";
        m_RecatMap["DC10"] = "C"; m_RecatMap["MD11"] = "C";
        // Military / Russian
        m_RecatMap["C17"] = "C"; // Globemaster
        m_RecatMap["IL76"] = "C"; m_RecatMap["IL96"] = "C";
        m_RecatMap["K35R"] = "C"; // KC-135 Stratotanker
		m_RecatMap["K10T"] = "C"; // KC-10 Extender
		m_RecatMap["E3TF"] = "C"; // E-3 Sentry (AWACS)

        // --- CAT D: UPPER MEDIUM ---
        // Boeing 757 
        m_RecatMap["B752"] = "D"; m_RecatMap["B753"] = "D";
        // Airbus Narrowbody
        m_RecatMap["A318"] = "D"; m_RecatMap["A319"] = "D"; m_RecatMap["A320"] = "D"; m_RecatMap["A321"] = "D";
        m_RecatMap["A19N"] = "D"; m_RecatMap["A20N"] = "D"; m_RecatMap["A21N"] = "D"; // Neos
        m_RecatMap["A21X"] = "D"; // A321 XLR
        // Boeing Narrowbody
        m_RecatMap["B732"] = "D"; m_RecatMap["B733"] = "D"; m_RecatMap["B734"] = "D"; m_RecatMap["B735"] = "D";
        m_RecatMap["B736"] = "D"; m_RecatMap["B737"] = "D"; m_RecatMap["B738"] = "D"; m_RecatMap["B739"] = "D";
        m_RecatMap["B37M"] = "D"; m_RecatMap["B38M"] = "D"; m_RecatMap["B39M"] = "D"; m_RecatMap["B3XM"] = "D"; // MAX
        m_RecatMap["P8"] = "D"; // Poseidon
        // Mad Dogs
        m_RecatMap["MD80"] = "D"; m_RecatMap["MD81"] = "D"; m_RecatMap["MD82"] = "D";
        m_RecatMap["MD83"] = "D"; m_RecatMap["MD88"] = "D"; m_RecatMap["MD90"] = "D";
        m_RecatMap["B712"] = "D"; // Boeing 717
        // Others
        m_RecatMap["T204"] = "D"; // Tu-204
        m_RecatMap["C130"] = "D"; m_RecatMap["L100"] = "D"; // Hercules
        m_RecatMap["C30J"] = "D"; // Super Hercules

        // --- CAT E: LOWER MEDIUM ---
        // Regional Jets
        m_RecatMap["BCS1"] = "E"; m_RecatMap["BCS3"] = "E"; // CSeries
        m_RecatMap["A221"] = "E"; m_RecatMap["A223"] = "E"; // A220
        m_RecatMap["E170"] = "E"; m_RecatMap["E175"] = "E";
        m_RecatMap["E190"] = "E"; m_RecatMap["E195"] = "E";
        m_RecatMap["E290"] = "E"; m_RecatMap["E295"] = "E"; // E2 Jets
        m_RecatMap["E135"] = "E"; m_RecatMap["E145"] = "E"; m_RecatMap["E14X"] = "E";
        m_RecatMap["CRJ1"] = "E"; m_RecatMap["CRJ2"] = "E";
        m_RecatMap["CRJ7"] = "E"; m_RecatMap["CRJ9"] = "E"; m_RecatMap["CRJX"] = "E"; // CRJ 1000
        m_RecatMap["F70"] = "E"; m_RecatMap["F100"] = "E";
        m_RecatMap["RJ85"] = "E"; m_RecatMap["RJ1H"] = "E"; m_RecatMap["BA46"] = "E"; // BAe 146 / Avro
        // Turboprops (Large)
        m_RecatMap["DH8A"] = "E"; m_RecatMap["DH8B"] = "E"; m_RecatMap["DH8C"] = "E"; m_RecatMap["DH8D"] = "E"; // Dash 8
        m_RecatMap["AT43"] = "E"; m_RecatMap["AT45"] = "E"; m_RecatMap["AT46"] = "E";
        m_RecatMap["AT72"] = "E"; m_RecatMap["AT73"] = "E"; m_RecatMap["AT75"] = "E"; m_RecatMap["AT76"] = "E";
        m_RecatMap["SB20"] = "E"; // Saab 2000
        m_RecatMap["CL2T"] = "E"; m_RecatMap["CL41"] = "E"; // Canadair Firefighters
        // Business Jets (Heavy/Long Range)
        m_RecatMap["GLF4"] = "E"; m_RecatMap["GLF5"] = "E"; m_RecatMap["GLF6"] = "E";
        m_RecatMap["GLEX"] = "E"; m_RecatMap["GL5T"] = "E"; m_RecatMap["GL7T"] = "E"; // Gulfstream
        m_RecatMap["FA7X"] = "E"; m_RecatMap["FA8X"] = "E"; // Falcon 7X/8X
        m_RecatMap["CL60"] = "E"; m_RecatMap["CL30"] = "E"; m_RecatMap["CL35"] = "E"; // Challenger
        m_RecatMap["C750"] = "E"; // Citation X
		// Others
		m_RecatMap["RFAL"] = "E"; m_RecatMap["EUFI"] = "E";

        // --- CAT F: LIGHT ---
        // General Aviation (Single Engine)
        m_RecatMap["C150"] = "F"; m_RecatMap["C152"] = "F"; m_RecatMap["C172"] = "F"; m_RecatMap["C182"] = "F";
        m_RecatMap["C206"] = "F"; m_RecatMap["C208"] = "F"; // Caravan is Light in RECAT (but usually wake turbulent!)
        m_RecatMap["PA28"] = "F"; m_RecatMap["PA32"] = "F"; m_RecatMap["PA34"] = "F"; m_RecatMap["PA44"] = "F";
        m_RecatMap["SR20"] = "F"; m_RecatMap["SR22"] = "F"; m_RecatMap["SF50"] = "F"; // Vision Jet
        m_RecatMap["DA20"] = "F"; m_RecatMap["DA40"] = "F"; m_RecatMap["DA42"] = "F"; m_RecatMap["DA62"] = "F";
        // GA (High Performance / Turboprop)
        m_RecatMap["TBM7"] = "F"; m_RecatMap["TBM8"] = "F"; m_RecatMap["TBM9"] = "F";
        m_RecatMap["PC12"] = "F"; m_RecatMap["PC24"] = "F";
        m_RecatMap["BE36"] = "F"; m_RecatMap["BE58"] = "F";
        m_RecatMap["BE9L"] = "F"; m_RecatMap["BE20"] = "F"; m_RecatMap["BE30"] = "F"; m_RecatMap["BE35"] = "F"; // King Airs
        // Business Jets (Small/Mid)
        m_RecatMap["C510"] = "F"; // Mustang
        m_RecatMap["C525"] = "F"; m_RecatMap["C25A"] = "F"; m_RecatMap["C25B"] = "F"; m_RecatMap["C25C"] = "F"; // CJ Series
        m_RecatMap["C550"] = "F"; m_RecatMap["C560"] = "F"; m_RecatMap["C56X"] = "F"; // Citation Excel
        m_RecatMap["C680"] = "F"; // Sovereign
        m_RecatMap["E50P"] = "F"; m_RecatMap["E55P"] = "F"; // Phenom 100/300
        m_RecatMap["LJ35"] = "F"; m_RecatMap["LJ45"] = "F"; m_RecatMap["LJ60"] = "F"; m_RecatMap["LJ75"] = "F"; // Learjet
        m_RecatMap["H25B"] = "F"; // Hawker 800XP
        // Misc
        m_RecatMap["P28A"] = "F"; m_RecatMap["P28R"] = "F"; m_RecatMap["P28T"] = "F";
        m_RecatMap["M20P"] = "F"; m_RecatMap["M20T"] = "F"; // Mooney
    }
};

// --- DLL Entry Points ---

CRecatPlugin* g_pPlugin = nullptr;

void __declspec (dllexport) EuroScopePlugInInit(EuroScopePlugIn::CPlugIn** ppPlugInInstance)
{
    g_pPlugin = new CRecatPlugin();
    *ppPlugInInstance = g_pPlugin;
}

void __declspec (dllexport) EuroScopePlugInExit(void)
{
    delete g_pPlugin;
}