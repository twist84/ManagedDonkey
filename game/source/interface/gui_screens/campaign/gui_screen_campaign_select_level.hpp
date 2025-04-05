#include "cseries/cseries.hpp"
#include "interface/c_gui_screen_widget.hpp"

enum e_controller_index;
enum e_gui_campaign_level_setup_mode;
struct c_gui_screen_campaign_select_level :
	public c_gui_screen_widget
{
public:
	void setup(e_gui_campaign_level_setup_mode campaign_setup_mode, long campaign_id, long map_id, short campaign_insertion_point);

//protected:
	e_gui_campaign_level_setup_mode m_campaign_setup_mode;
	long m_campaign_id;
	long m_map_id;
	short m_campaign_insertion_point;
	e_controller_index m_controller_index;
	bool m_saved_game;
	bool m_coop_saved_game;
};
static_assert(sizeof(c_gui_screen_campaign_select_level) == sizeof(c_gui_screen_widget) + 0x18);

