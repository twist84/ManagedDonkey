#include "cseries/cseries.hpp"
#include "interface/c_gui_screen_widget.hpp"

enum e_campaign_difficulty_level;
enum e_gui_campaign_difficulty_setup_mode;
struct c_gui_screen_campaign_select_difficulty :
	public c_gui_screen_widget
{
public:
	void setup(e_gui_campaign_difficulty_setup_mode campaign_setup_mode, long campaign_id, long map_id, e_campaign_difficulty_level difficulty);

//protected:
	e_gui_campaign_difficulty_setup_mode m_campaign_setup_mode;
	long m_campaign_id;
	long m_map_id;
	e_campaign_difficulty_level m_difficulty;
};
static_assert(sizeof(c_gui_screen_campaign_select_difficulty) == sizeof(c_gui_screen_widget) + 0x10);

