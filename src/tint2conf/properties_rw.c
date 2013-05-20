
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#include "common.h"
#include "properties.h"
#include "properties_rw.h"


void add_entry (char *key, char *value);
void hex2gdk(char *hex, GdkColor *color);
void get_action(char *event, GtkWidget *combo);



void config_read_file (const char *path)
{
	FILE *fp;
	char line[512];
	char *key, *value;

	if ((fp = fopen(path, "r")) == NULL) return;

	while (fgets(line, sizeof(line), fp) != NULL) {
		if (parse_line(line, &key, &value)) {
			add_entry (key, value);
			free (key);
			free (value);
		}
	}
	fclose (fp);
}


void config_save_file(const char *path) {
	//printf("config_save_file : %s\n", path);
}


void add_entry (char *key, char *value)
{
	char *value1=0, *value2=0, *value3=0;

	/* Background and border */
	if (strcmp (key, "rounded") == 0) {
		// 'rounded' is the first parameter => alloc a new background
		//Background bg;
		//bg.border.rounded = atoi(value);
		//g_array_append_val(backgrounds, bg);
	}
	else if (strcmp (key, "border_width") == 0) {
		//g_array_index(backgrounds, Background, backgrounds->len-1).border.width = atoi(value);
	}
	else if (strcmp (key, "background_color") == 0) {
		extract_values(value, &value1, &value2, &value3);
		//Background* bg = &g_array_index(backgrounds, Background, backgrounds->len-1);
		//get_color (value1, bg->back.color);
		//if (value2) bg->back.alpha = (atoi (value2) / 100.0);
		//else bg->back.alpha = 0.5;
	}
	else if (strcmp (key, "border_color") == 0) {
		extract_values(value, &value1, &value2, &value3);
		//Background* bg = &g_array_index(backgrounds, Background, backgrounds->len-1);
		//get_color (value1, bg->border.color);
		//if (value2) bg->border.alpha = (atoi (value2) / 100.0);
		//else bg->border.alpha = 0.5;
	}

	/* Panel */
	else if (strcmp (key, "panel_size") == 0) {
		extract_values(value, &value1, &value2, &value3);
		char *b;
		if ((b = strchr (value1, '%'))) {
			b[0] = '\0';
			gtk_combo_box_set_active(GTK_COMBO_BOX(panel_combo_width_type), 0);
		}
		else
			gtk_combo_box_set_active(GTK_COMBO_BOX(panel_combo_width_type), 1);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(panel_width), atof(value1));
		if (atoi(value1) == 0) {
			// full width mode
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(panel_width), 100);
			gtk_combo_box_set_active(GTK_COMBO_BOX(panel_combo_width_type), 0);
		}
		if ((b = strchr (value2, '%'))) {
			b[0] = '\0';
			gtk_combo_box_set_active(GTK_COMBO_BOX(panel_combo_height_type), 0);
		}
		else
			gtk_combo_box_set_active(GTK_COMBO_BOX(panel_combo_height_type), 1);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(panel_height), atof(value2));
	}
	else if (strcmp (key, "panel_items") == 0) {
		gtk_entry_set_text(GTK_ENTRY(items_order), value);
	}
	else if (strcmp (key, "panel_margin") == 0) {
		extract_values(value, &value1, &value2, &value3);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(panel_margin_x), atof(value1));
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(panel_margin_y), atof(value1));
	}
	else if (strcmp (key, "panel_padding") == 0) {
		extract_values(value, &value1, &value2, &value3);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(panel_padding_x), atof(value1));
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(panel_spacing), atof(value1));
		if (value2) gtk_spin_button_set_value(GTK_SPIN_BUTTON(panel_padding_y), atof(value2));
		if (value3) gtk_spin_button_set_value(GTK_SPIN_BUTTON(panel_spacing), atof(value3));
	}
	else if (strcmp (key, "panel_position") == 0) {
		extract_values(value, &value1, &value2, &value3);
		/*
		if (strcmp (value1, "top") == 0) panel_position = TOP;
		else {
			if (strcmp (value1, "bottom") == 0) panel_position = BOTTOM;
			else panel_position = CENTER;
		}

		if (!value2) panel_position |= CENTER;
		else {
			if (strcmp (value2, "left") == 0) panel_position |= LEFT;
			else {
				if (strcmp (value2, "right") == 0) panel_position |= RIGHT;
				else panel_position |= CENTER;
			}
		}
		if (!value3) panel_horizontal = 1;
		else {
			if (strcmp (value3, "vertical") == 0) panel_horizontal = 0;
			else panel_horizontal = 1;
		}
		*/
	}
	else if (strcmp (key, "panel_background_id") == 0) {
		//int id = atoi (value);
		//id = (id < backgrounds->len && id >= 0) ? id : 0;
		//panel_config.area.bg = &g_array_index(backgrounds, Background, id);
	}
	else if (strcmp (key, "wm_menu") == 0) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(panel_wm_menu), atoi(value));
	}
	else if (strcmp (key, "panel_dock") == 0) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(panel_dock), atoi(value));
	}
	else if (strcmp (key, "panel_layer") == 0) {
		if (strcmp(value, "bottom") == 0)
			gtk_combo_box_set_active(GTK_COMBO_BOX(panel_combo_layer), 2);
		else if (strcmp(value, "top") == 0)
			gtk_combo_box_set_active(GTK_COMBO_BOX(panel_combo_layer), 0);
		else
			gtk_combo_box_set_active(GTK_COMBO_BOX(panel_combo_layer), 1);
	}
	else if (strcmp (key, "panel_monitor") == 0) {
		//panel_config.monitor = config_get_monitor(value);
	}
	
	/* autohide options */
	else if (strcmp(key, "autohide") == 0) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(panel_autohide), atoi(value));
	}
	else if (strcmp(key, "autohide_show_timeout") == 0) {
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(panel_autohide_show_time), atof(value));
	}
	else if (strcmp(key, "autohide_hide_timeout") == 0) {
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(panel_autohide_hide_time), atof(value));
	}
	else if (strcmp(key, "strut_policy") == 0) {
		if (strcmp(value, "follow_size") == 0)
			gtk_combo_box_set_active(GTK_COMBO_BOX(panel_combo_strut_policy), 0);
		else if (strcmp(value, "none") == 0)
			gtk_combo_box_set_active(GTK_COMBO_BOX(panel_combo_strut_policy), 2);
		else
			gtk_combo_box_set_active(GTK_COMBO_BOX(panel_combo_strut_policy), 1);
	}
	else if (strcmp(key, "autohide_height") == 0) {
		if (atoi(value) == 0) {
			// autohide need height > 0
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(panel_autohide_size), 1);
		}
		else
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(panel_autohide_size), atof(value));
	}

	/* Battery */
	else if (strcmp (key, "battery_low_status") == 0) {
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(battery_alert_if_lower), atof(value));
	}
	else if (strcmp (key, "battery_low_cmd") == 0) {
		gtk_entry_set_text(GTK_ENTRY(battery_alert_cmd), value);
	}
	else if (strcmp (key, "bat1_font") == 0) {
		gtk_font_button_set_font_name(GTK_FONT_BUTTON(battery_font_line1), value);
	}
	else if (strcmp (key, "bat2_font") == 0) {
		gtk_font_button_set_font_name(GTK_FONT_BUTTON(battery_font_line2), value);
	}
	else if (strcmp (key, "battery_font_color") == 0) {
		extract_values(value, &value1, &value2, &value3);
		GdkColor col;
		hex2gdk(value1, &col);
		gtk_color_button_set_color(GTK_COLOR_BUTTON(battery_font_color), &col);
		if (value2) {
			int alpha = atoi(value2);
			gtk_color_button_set_alpha(GTK_COLOR_BUTTON(battery_font_color), (alpha*65535)/100);
		}
	}
	else if (strcmp (key, "battery_padding") == 0) {
		extract_values(value, &value1, &value2, &value3);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(battery_padding_x), atof(value1));
		if (value2) gtk_spin_button_set_value(GTK_SPIN_BUTTON(battery_padding_y), atof(value2));
	}
	else if (strcmp (key, "battery_background_id") == 0) {
		//int id = atoi (value);
		//id = (id < backgrounds->len && id >= 0) ? id : 0;
		//panel_config.battery.area.bg = &g_array_index(backgrounds, Background, id);
	}
	else if (strcmp (key, "battery_hide") == 0) {
		int percentage_hide = atoi (value);
		if (percentage_hide == 0)
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(battery_hide_if_higher), 101.0);
		else
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(battery_hide_if_higher), atof(value));
	}

	/* Clock */
	else if (strcmp (key, "time1_format") == 0) {
		gtk_entry_set_text(GTK_ENTRY(clock_format_line1), value);
	}
	else if (strcmp (key, "time2_format") == 0) {
		gtk_entry_set_text(GTK_ENTRY(clock_format_line2), value);
	}
	else if (strcmp (key, "time1_font") == 0) {
		gtk_font_button_set_font_name(GTK_FONT_BUTTON(clock_font_line1), value);
	}
	else if (strcmp(key, "time1_timezone") == 0) {
		gtk_entry_set_text(GTK_ENTRY(clock_tmz_line1), value);
	}
	else if (strcmp(key, "time2_timezone") == 0) {
		gtk_entry_set_text(GTK_ENTRY(clock_tmz_line2), value);
	}
	else if (strcmp (key, "time2_font") == 0) {
		gtk_font_button_set_font_name(GTK_FONT_BUTTON(clock_font_line2), value);
	}
	else if (strcmp (key, "clock_font_color") == 0) {
		extract_values(value, &value1, &value2, &value3);
		GdkColor col;
		hex2gdk(value1, &col);
		gtk_color_button_set_color(GTK_COLOR_BUTTON(clock_font_color), &col);
		if (value2) {
			int alpha = atoi(value2);
			gtk_color_button_set_alpha(GTK_COLOR_BUTTON(clock_font_color), (alpha*65535)/100);
		}
	}
	else if (strcmp (key, "clock_padding") == 0) {
		extract_values(value, &value1, &value2, &value3);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(clock_padding_x), atof(value1));
		if (value2) gtk_spin_button_set_value(GTK_SPIN_BUTTON(clock_padding_y), atof(value2));
	}
	else if (strcmp (key, "clock_background_id") == 0) {
		//int id = atoi (value);
		//id = (id < backgrounds->len && id >= 0) ? id : 0;
		//panel_config.clock.area.bg = &g_array_index(backgrounds, Background, id);
	}
	else if (strcmp(key, "clock_tooltip") == 0) {
		gtk_entry_set_text(GTK_ENTRY(clock_format_tooltip), value);
	}
	else if (strcmp(key, "clock_tooltip_timezone") == 0) {
		gtk_entry_set_text(GTK_ENTRY(clock_tmz_tooltip), value);
	}
	else if (strcmp(key, "clock_lclick_command") == 0) {
		gtk_entry_set_text(GTK_ENTRY(clock_left_command), value);
	}
	else if (strcmp(key, "clock_rclick_command") == 0) {
		gtk_entry_set_text(GTK_ENTRY(clock_right_command), value);
	}

	/* Taskbar */
	else if (strcmp (key, "taskbar_mode") == 0) {
		if (strcmp (value, "multi_desktop") == 0)
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(taskbar_show_desktop), 1);
		else
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(taskbar_show_desktop), 0);
	}
	else if (strcmp (key, "taskbar_padding") == 0) {
		extract_values(value, &value1, &value2, &value3);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(taskbar_padding_x), atof(value1));
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(taskbar_spacing), atof(value1));
		if (value2) gtk_spin_button_set_value(GTK_SPIN_BUTTON(taskbar_padding_y), atof(value2));
		if (value3) gtk_spin_button_set_value(GTK_SPIN_BUTTON(taskbar_spacing), atof(value3));
	}
	else if (strcmp (key, "taskbar_background_id") == 0) {
		//int id = atoi (value);
		//id = (id < backgrounds->len && id >= 0) ? id : 0;
		//panel_config.g_taskbar.background[TASKBAR_NORMAL] = &g_array_index(backgrounds, Background, id);
		//if (panel_config.g_taskbar.background[TASKBAR_ACTIVE] == 0)
			//panel_config.g_taskbar.background[TASKBAR_ACTIVE] = panel_config.g_taskbar.background[TASKBAR_NORMAL];
	}
	else if (strcmp (key, "taskbar_active_background_id") == 0) {
		//int id = atoi (value);
		//id = (id < backgrounds->len && id >= 0) ? id : 0;
		//panel_config.g_taskbar.background[TASKBAR_ACTIVE] = &g_array_index(backgrounds, Background, id);
	}
	else if (strcmp (key, "taskbar_name") == 0) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(taskbar_show_name), atoi(value));
	}
	else if (strcmp (key, "taskbar_name_padding") == 0) {
		extract_values(value, &value1, &value2, &value3);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(taskbar_name_padding_x), atof(value1));
	}
	else if (strcmp (key, "taskbar_name_background_id") == 0) {
		//int id = atoi (value);
		//id = (id < backgrounds->len && id >= 0) ? id : 0;
		//panel_config.g_taskbar.background_name[TASKBAR_NORMAL] = &g_array_index(backgrounds, Background, id);
		//if (panel_config.g_taskbar.background_name[TASKBAR_ACTIVE] == 0)
			//panel_config.g_taskbar.background_name[TASKBAR_ACTIVE] = panel_config.g_taskbar.background_name[TASKBAR_NORMAL];
	}
	else if (strcmp (key, "taskbar_name_active_background_id") == 0) {
		//int id = atoi (value);
		//id = (id < backgrounds->len && id >= 0) ? id : 0;
		//panel_config.g_taskbar.background_name[TASKBAR_ACTIVE] = &g_array_index(backgrounds, Background, id);
	}
	else if (strcmp (key, "taskbar_name_font") == 0) {
		gtk_font_button_set_font_name(GTK_FONT_BUTTON(taskbar_name_font), value);
	}
	else if (strcmp (key, "taskbar_name_font_color") == 0) {
		extract_values(value, &value1, &value2, &value3);
		GdkColor col;
		hex2gdk(value1, &col);
		gtk_color_button_set_color(GTK_COLOR_BUTTON(taskbar_name_inactive_color), &col);
		if (value2) {
			int alpha = atoi(value2);
			gtk_color_button_set_alpha(GTK_COLOR_BUTTON(taskbar_name_inactive_color), (alpha*65535)/100);
		}
	}
	else if (strcmp (key, "taskbar_name_active_font_color") == 0) {
		extract_values(value, &value1, &value2, &value3);
		GdkColor col;
		hex2gdk(value1, &col);
		gtk_color_button_set_color(GTK_COLOR_BUTTON(taskbar_name_active_color), &col);
		if (value2) {
			int alpha = atoi(value2);
			gtk_color_button_set_alpha(GTK_COLOR_BUTTON(taskbar_name_active_color), (alpha*65535)/100);
		}
	}

	/* Task */
	else if (strcmp (key, "task_text") == 0) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(task_show_text), atoi(value));
	}
	else if (strcmp (key, "task_icon") == 0) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(task_show_icon), atoi(value));
	}
	else if (strcmp (key, "task_centered") == 0) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(task_align_center), atoi(value));
	}
	else if (strcmp (key, "font_shadow") == 0) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(task_font_shadow), atoi(value));
	}
	else if (strcmp (key, "urgent_nb_of_blink") == 0) {
		//max_tick_urgent = atoi (value);
	}
	else if (strcmp (key, "task_width") == 0) {
		// old parameter : just for backward compatibility
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(task_maximum_width), atof(value));
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(task_maximum_height), 30.0);
	}
	else if (strcmp (key, "task_maximum_size") == 0) {
		extract_values(value, &value1, &value2, &value3);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(task_maximum_width), atof(value1));
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(task_maximum_height), 30.0);
		if (value2)
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(task_maximum_height), atof(value2));
	}
	else if (strcmp (key, "task_padding") == 0) {
		extract_values(value, &value1, &value2, &value3);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(task_padding_x), atof(value1));
		if (value2) gtk_spin_button_set_value(GTK_SPIN_BUTTON(task_padding_y), atof(value2));
	}
	else if (strcmp (key, "task_font") == 0) {
		gtk_font_button_set_font_name(GTK_FONT_BUTTON(task_font), value);
	}
	else if (g_regex_match_simple("task.*_font_color", key, 0, 0)) {
		/*gchar** split = g_regex_split_simple("_", key, 0, 0);
		int status = get_task_status(split[1]);
		g_strfreev(split);
		extract_values(value, &value1, &value2, &value3);
		float alpha = 1;
		if (value2) alpha = (atoi (value2) / 100.0);
		get_color (value1, panel_config.g_task.font[status].color);
		panel_config.g_task.font[status].alpha = alpha;
		panel_config.g_task.config_font_mask |= (1<<status);
		*/
	}
	else if (g_regex_match_simple("task.*_icon_asb", key, 0, 0)) {
		/*gchar** split = g_regex_split_simple("_", key, 0, 0);
		int status = get_task_status(split[1]);
		g_strfreev(split);
		extract_values(value, &value1, &value2, &value3);
		panel_config.g_task.alpha[status] = atoi(value1);
		panel_config.g_task.saturation[status] = atoi(value2);
		panel_config.g_task.brightness[status] = atoi(value3);
		panel_config.g_task.config_asb_mask |= (1<<status);
		*/
	}
	else if (g_regex_match_simple("task.*_background_id", key, 0, 0)) {
		/*gchar** split = g_regex_split_simple("_", key, 0, 0);
		int status = get_task_status(split[1]);
		g_strfreev(split);
		int id = atoi (value);
		id = (id < backgrounds->len && id >= 0) ? id : 0;
		panel_config.g_task.background[status] = &g_array_index(backgrounds, Background, id);
		panel_config.g_task.config_background_mask |= (1<<status);
		if (status == TASK_NORMAL) panel_config.g_task.area.bg = panel_config.g_task.background[TASK_NORMAL];
		*/
	}
	// "tooltip" is deprecated but here for backwards compatibility
	else if (strcmp (key, "task_tooltip") == 0 || strcmp(key, "tooltip") == 0) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tooltip_task_show), atoi(value));
	}

	/* Systray */
	else if (strcmp (key, "systray_padding") == 0) {
		extract_values(value, &value1, &value2, &value3);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(systray_padding_x), atof(value1));
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(systray_spacing), atof(value1));
		if (value2) gtk_spin_button_set_value(GTK_SPIN_BUTTON(systray_padding_y), atof(value2));
		if (value3) gtk_spin_button_set_value(GTK_SPIN_BUTTON(systray_spacing), atof(value3));
	}
	else if (strcmp (key, "systray_background_id") == 0) {
		//int id = atoi (value);
		//id = (id < backgrounds->len && id >= 0) ? id : 0;
		//systray.area.bg = &g_array_index(backgrounds, Background, id);
	}
	else if (strcmp(key, "systray_sort") == 0) {
		if (strcmp(value, "descending") == 0)
			gtk_combo_box_set_active(GTK_COMBO_BOX(systray_icon_order), 1);
		else if (strcmp(value, "ascending") == 0)
			gtk_combo_box_set_active(GTK_COMBO_BOX(systray_icon_order), 0);
		else if (strcmp(value, "right2left") == 0)
			gtk_combo_box_set_active(GTK_COMBO_BOX(systray_icon_order), 3);
		else  // default to left2right
			gtk_combo_box_set_active(GTK_COMBO_BOX(systray_icon_order), 2);
	}
	else if (strcmp(key, "systray_icon_size") == 0) {
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(systray_icon_size), atof(value));
	}
	else if (strcmp(key, "systray_icon_asb") == 0) {
		extract_values(value, &value1, &value2, &value3);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(systray_icon_opacity), atof(value1));
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(systray_icon_saturation), atof(value2));
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(systray_icon_brightness), atof(value3));
	}

	/* Launcher */
	else if (strcmp (key, "launcher_padding") == 0) {
		extract_values(value, &value1, &value2, &value3);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(launcher_padding_x), atof(value1));
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(launcher_spacing), atof(value1));
		if (value2) gtk_spin_button_set_value(GTK_SPIN_BUTTON(launcher_padding_y), atof(value2));
		if (value3) gtk_spin_button_set_value(GTK_SPIN_BUTTON(launcher_spacing), atof(value3));
	}
	else if (strcmp (key, "launcher_background_id") == 0) {
		//int id = atoi (value);
		//id = (id < backgrounds->len && id >= 0) ? id : 0;
		//panel_config.launcher.area.bg = &g_array_index(backgrounds, Background, id);
	}
	else if (strcmp(key, "launcher_icon_size") == 0) {
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(launcher_icon_size), atof(value));
	}
	else if (strcmp(key, "launcher_item_app") == 0) {
		//char *app = strdup(value);
		//panel_config.launcher.list_apps = g_slist_append(panel_config.launcher.list_apps, app);
	}
	else if (strcmp(key, "launcher_icon_theme") == 0) {
		// if XSETTINGS manager running, tint2 use it.
		//icon_theme_name = strdup(value);
	}

	/* Tooltip */
	else if (strcmp (key, "tooltip_show_timeout") == 0) {
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(tooltip_show_after), atof(value));
	}
	else if (strcmp (key, "tooltip_hide_timeout") == 0) {
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(tooltip_hide_after), atof(value));
	}
	else if (strcmp (key, "tooltip_padding") == 0) {
		extract_values(value, &value1, &value2, &value3);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(tooltip_padding_x), atof(value1));
		if (value2) gtk_spin_button_set_value(GTK_SPIN_BUTTON(tooltip_padding_y), atof(value2));
	}
	else if (strcmp (key, "tooltip_background_id") == 0) {
		//int id = atoi (value);
		//id = (id < backgrounds->len && id >= 0) ? id : 0;
		//g_tooltip.bg = &g_array_index(backgrounds, Background, id);
	}
	else if (strcmp (key, "tooltip_font_color") == 0) {
		extract_values(value, &value1, &value2, &value3);
		GdkColor col;
		hex2gdk(value1, &col);
		gtk_color_button_set_color(GTK_COLOR_BUTTON(tooltip_font_color), &col);
		if (value2) {
			int alpha = atoi(value2);
			gtk_color_button_set_alpha(GTK_COLOR_BUTTON(tooltip_font_color), (alpha*65535)/100);
		}
	}
	else if (strcmp (key, "tooltip_font") == 0) {
		gtk_font_button_set_font_name(GTK_FONT_BUTTON(tooltip_font), value);
	}

	/* Mouse actions */
	else if (strcmp (key, "mouse_middle") == 0) {
		get_action(value, task_mouse_middle);
	}
	else if (strcmp (key, "mouse_right") == 0) {
		get_action(value, task_mouse_right);
	}
	else if (strcmp (key, "mouse_scroll_up") == 0) {
		get_action(value, task_mouse_scroll_up);
	}
	else if (strcmp (key, "mouse_scroll_down") == 0) {
		get_action(value, task_mouse_scroll_down);
	}

	if (value1) free (value1);
	if (value2) free (value2);
	if (value3) free (value3);
}


void hex2gdk(char *hex, GdkColor *color)
{
	if (hex == NULL || hex[0] != '#') return;

	color->red   = 257 * (hex_char_to_int (hex[1]) * 16 + hex_char_to_int (hex[2]));
	color->green = 257 * (hex_char_to_int (hex[3]) * 16 + hex_char_to_int (hex[4]));
	color->blue  = 257 * (hex_char_to_int (hex[5]) * 16 + hex_char_to_int (hex[6]));
}


void get_action(char *event, GtkWidget *combo)
{
	if (strcmp (event, "none") == 0)
		gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
	else if (strcmp (event, "close") == 0)
		gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 1);
	else if (strcmp (event, "toggle") == 0)
		gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 2);
	else if (strcmp (event, "iconify") == 0)
		gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 3);
	else if (strcmp (event, "shade") == 0)
		gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 4);
	else if (strcmp (event, "toggle_iconify") == 0)
		gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 5);
	else if (strcmp (event, "maximize_restore") == 0)
		gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 6);
	else if (strcmp (event, "desktop_left") == 0)
		gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 7);
	else if (strcmp (event, "desktop_right") == 0)
		gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 8);
	else if (strcmp (event, "next_task") == 0)
		gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 9);
	else if (strcmp (event, "prev_task") == 0)
		gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 10);
}

