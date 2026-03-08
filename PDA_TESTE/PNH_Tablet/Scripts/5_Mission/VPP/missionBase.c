const int VPP_MENU_MAP = 19943; 

modded class MissionBase {
    override UIScriptedMenu CreateScriptedMenu(int id) {
        UIScriptedMenu menu = NULL;
        menu = super.CreateScriptedMenu(id);
        if (!menu) {
            switch (id) {
            case VPP_MENU_MAP:
                menu = new PNH_MapUI; // <--- AGORA ELE ABRE A NOVA UI
                break;
            }
            if (menu) {
                menu.SetID(id);
            }
        }
        return menu;
    }
}