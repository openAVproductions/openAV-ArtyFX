/*
 * Author: Harry van Haaren 2014
 *         harryhaaren@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

extern LV2UI_Handle friza_instantiate(const struct LV2UI_Descriptor * descriptor,
                                      const char * plugin_uri,
                                      const char * bundle_path,
                                      LV2UI_Write_Function write_function,
                                      LV2UI_Controller controller,
                                      LV2UI_Widget * widget,
                                      const LV2_Feature * const * features);

extern void friza_cleanup(LV2UI_Handle ui);

extern void friza_port_event(LV2UI_Handle ui,
                             uint32_t port_index,
                             uint32_t buffer_size,
                             uint32_t format,
                             const void * buffer);

extern int friza_idle(LV2UI_Handle handle);

extern const void* friza_extension_data(const char* uri);


