// Ficheiro: PNH_Tablet/Scripts/5_Mission/PNH_Tablet/Core/PNH_TabletController.c

modded class MissionGameplay {

    private ref PNH_MapUI m_PNHMapMenu;

    override void OnUpdate(float timeslice) {
        super.OnUpdate(timeslice);

        // Atalho para o Mapa (Tecla M)
        if (GetUApi().GetInputByName("UAUIBack").LocalPress() || (GetGame().GetInput().LocalPress("UAVPPMapOpen", false) && !GetGame().GetUIManager().GetMenu())) {
            TogglePNHMap();
        }
    }

    void TogglePNHMap() {
        if (!m_PNHMapMenu) {
            m_PNHMapMenu = PNH_MapUI.Cast(GetGame().GetUIManager().EnterScriptedMenu(PNH_PR_MENU_MAP, null));
        } else {
            if (GetGame().GetUIManager().GetMenu() == m_PNHMapMenu) {
                GetGame().GetUIManager().HideScriptedMenu(m_PNHMapMenu);
            } else {
                GetGame().GetUIManager().ShowScriptedMenu(m_PNHMapMenu, null);
            }
        }
    }
}

// Constantes de Identificação do Menu
const int PNH_PR_MENU_MAP = 991024;