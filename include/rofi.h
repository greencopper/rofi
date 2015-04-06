#ifndef __SIMPLESWITCHER_H__
#define __SIMPLESWITCHER_H__
#include <X11/X.h>
#include <glib.h>
#include <textbox.h>


/**
 * Pointer to xdg cache directory.
 */
extern const char *cache_dir;

typedef struct _Switcher   Switcher;
/**
 * Enum used to sum the possible states of ROFI.
 */
typedef enum
{
    /** Exit. */
    MODE_EXIT       = 1000,
    /** Skip to the next cycle-able dialog. */
    NEXT_DIALOG     = 1001,
    /** Reload current DIALOG */
    RELOAD_DIALOG   = 1002,
    /** Previous dialog */
    PREVIOUS_DIALOG = 1003
} SwitcherMode;

/**
 * @param input Pointer to the user input.
 * @param data  Usr data.
 *
 * Callback typedef for a switcher
 *
 * @returns SwitcherMode
 */
typedef SwitcherMode ( *switcher_callback )( char **input, void *data );
typedef void ( *switcher_free )( Switcher *data );


typedef const char * ( *get_display_value )( unsigned int selected_line, void *data, int *state );
/**
 * State returned by the rofi window.
 */
typedef enum
{
    /** Entry is selected. */
    MENU_OK           = 0x1,
    /** User canceled the operation. (e.g. pressed escape) */
    MENU_CANCEL       = 0x2,
    /** User requested a mode switch */
    MENU_NEXT         = 0x4,
    /** Custom (non-matched) input was entered. */
    MENU_CUSTOM_INPUT = 0x8,
    /** User wanted to delete entry from history. */
    MENU_ENTRY_DELETE = 0x10,
    /** User wants to jump to another switcher. */
    MENU_QUICK_SWITCH = 0x20,
    /** Go to the previous menu. */
    MENU_PREVIOUS     = 0x40,
    /** Modifiers */
    MENU_SHIFT        = 0x1000
} MenuReturn;


/**
 * @param tokens  List of (input) tokens to match.
 * @param input   The entry to match against.
 * @param case_sensitive Whether case is significant.
 * @param index   The current selected index.
 * @param data    User data.
 *
 * Function prototype for the matching algorithm.
 *
 * @returns 1 when it matches, 0 if not.
 */
typedef int ( *menu_match_cb )( char **tokens, const char *input, int case_sensitive, unsigned int index, Switcher *data );

/**
 * @param lines An array of strings to display.
 * @param num_lines Length of the array with strings to display.
 * @param input A pointer to a string where the inputted data is placed.
 * @param prompt The prompt to show.
 * @param shift pointer to integer that is set to the state of the shift key.
 * @param mmc Menu menu match callback, used for matching user input.
 * @param mmc_data data to pass to mmc.
 * @param selected_line pointer to integer holding the selected line.
 *
 * Main menu callback.
 *
 * @returns The command issued (see MenuReturn)
 */
MenuReturn menu ( char **lines, unsigned int num_lines, char **input, char *prompt,
                  menu_match_cb mmc, void *mmc_data,
                  int *selected_line, int sorting, get_display_value mgrv, void *mgrv_data ) __attribute__ ( ( nonnull ( 1, 3, 4, 7 ) ) );
/**
 * @param sig  The caught signal
 *
 * Catch the exit signal generated by X.
 */
void catch_exit ( __attribute__( ( unused ) ) int sig );

/**
 * Enumeration indicating location or gravity of window.
 *
 * WL_NORTH_WEST      WL_NORTH      WL_NORTH_EAST
 *
 * WL_EAST            WL_CENTER     WL_EAST
 *
 * WL_SOUTH_WEST      WL_SOUTH      WL_SOUTH_EAST
 *
 */
typedef enum _WindowLocation
{
    /** Center */
    WL_CENTER     = 0,
    /** Left top corner. */
    WL_NORTH_WEST = 1,
    /** Top middle */
    WL_NORTH      = 2,
    /** Top right */
    WL_NORTH_EAST = 3,
    /** Middle right */
    WL_EAST       = 4,
    /** Bottom right */
    WL_EAST_SOUTH = 5,
    /** Bottom middle */
    WL_SOUTH      = 6,
    /** Bottom left */
    WL_SOUTH_WEST = 7,
    /** Middle left */
    WL_WEST       = 8
} WindowLocation;

/**
 * Settings
 */

typedef struct _Settings
{
    /** List of enabled switchers */
    char           *switchers;
    /** Window settings */
    unsigned int   window_opacity;
    /** Border width */
    unsigned int   menu_bw;
    /** Width (0-100 in %, > 100 in pixels, < 0 in char width.) */
    int            menu_width;
    /** # lines */
    unsigned int   menu_lines;
    /** # Columns */
    unsigned int   menu_columns;
    /** Font string (pango format) */
    char           * menu_font;

    /** New row colors */
    char           * color_normal;
    char           * color_active;
    char           * color_urgent;
    char           * color_window;
    /** Foreground color */
    char           * menu_fg;
    char           * menu_fg_urgent;
    char           * menu_fg_active;
    /** Background color */
    char           * menu_bg;
    char           * menu_bg_urgent;
    char           * menu_bg_active;
    /** Background color alt */
    char           * menu_bg_alt;
    /** Highlight foreground color */
    char           * menu_hlfg;
    char           * menu_hlfg_urgent;
    char           * menu_hlfg_active;
    /** Highlight background color */
    char           * menu_hlbg;
    char           * menu_hlbg_urgent;
    char           * menu_hlbg_active;
    /** Border color */
    char           * menu_bc;
    /** Terminal to use  */
    char           * terminal_emulator;
    /** SSH client to use */
    char           * ssh_client;
    /** Command to execute when ssh session is selected */
    char           * ssh_command;
    /** Command for executing an application */
    char           * run_command;
    /** Command for executing an application in a terminal */
    char           * run_shell_command;
    /** Command for listing executables */
    char           * run_list_command;

    /** Windows location/gravity */
    WindowLocation location;
    /** Padding between elements */
    unsigned int   padding;
    /** Y offset */
    int            y_offset;
    /** X offset */
    int            x_offset;
    /** Always should config.menu_lines lines, even if less lines are available */
    unsigned int   fixed_num_lines;
    /** Do not use history */
    unsigned int   disable_history;
    /** Use levenshtein sorting when matching */
    unsigned int   levenshtein_sort;
    /** Search case sensitivity */
    unsigned int   case_sensitive;
    /** Separator to use for dmenu mode */
    char           separator;
    /** Height of an element in #chars */
    int            element_height;
    /** Sidebar mode, show the switchers */
    unsigned int   sidebar_mode;
    /** Lazy filter limit. */
    unsigned int   lazy_filter_limit;
    /** Auto select. */
    unsigned int   auto_select;
    /** Hosts file parsing */
    unsigned int   parse_hosts;
    /** Combi Switchers */
    char           *combi_modi;
} Settings;

/** Global Settings structure. */
extern Settings config;

/**
 * @param msg The error message to show.
 *
 * The error message to show.
 */
void error_dialog ( const char *msg );

/**
 * Structure defining a switcher.
 * It consists of a name, callback and if enabled
 * a textbox for the sidebar-mode.
 */
struct _Switcher
{
    // Name (max 31 char long)
    char         name[32];
    // Textbox used in the sidebar-mode.
    textbox      *tb;
    // Keybindings (keysym and modmask)
    char         * keycfg;
    char         * keystr;
    KeySym       keysym;
    unsigned int modmask;


    /**
     * A switcher normally consists of the following parts:
     */
    void              ( *init )( struct _Switcher *sw );
    char              ** ( *get_data )( unsigned int *length, struct _Switcher *pd );
    int               ( *match )( char **tokens, const char *input, int case_sensitive, int index, struct _Switcher *data );
    SwitcherMode      ( *result )( int menu_retv, char **input, unsigned int selected_line, struct _Switcher *pd );
    void              ( *destroy )( struct _Switcher *pd );
    // Token match.
    menu_match_cb     token_match;

    get_display_value mgrv;

    // Pointer to private data.
    void              *private_data;

    // Extra fields for script
    void              *ed;
    // Free SWitcher
    switcher_free     free;
};

void menu_unmap ( void );

#define  color_reset           "\033[0m"
#define  color_bold            "\033[1m"
#define  color_underline       "\033[4m"
#define  color_black           "\033[0;30m"
#define  color_red             "\033[0;31m"
#define  color_green           "\033[0;32m"
#define  color_yellow          "\033[0;33m"
#define  color_blue            "\033[0;34m"
#define  color_magenta         "\033[0;35m"
#define  color_cyan            "\033[0;36m"
#define  color_white           "\033[0;37m"
#define  color_white_bold      "\033[1;37m"
#define  color_black_bold      "\033[1;30m"
#define  color_red_bold        "\033[1;31m"
#define  color_green_bold      "\033[1;32m"
#define  color_yellow_bold     "\033[1;33m"
#define  color_blue_bold       "\033[1;34m"
#define  color_magenta_bold    "\033[1;35m"
#define  color_cyan_bold       "\033[1;36m"
#endif
