// This file is part of the Script Brushes extension of Noggit3 by TSWoW (https://github.com/tswow/)
// licensed under GNU General Public License (version 3).

// Note: This file should only be included by script_loader.cpp
#pragma once

#include <string>

#include <daScript/daScript.h>
#include <noggit/scripting/scripting_tool.hpp>

#include <noggit/scripting/script_image.hpp>
#include <noggit/scripting/script_context.hpp>
#include <noggit/scripting/script_filesystem.hpp>
#include <noggit/scripting/script_math.hpp>
#include <noggit/scripting/script_noise.hpp>
#include <noggit/scripting/script_random.hpp>
#include <noggit/scripting/script_selection.hpp>
#include <noggit/scripting/scripting_tool.hpp>
#include <noggit/scripting/script_brush.hpp>
#include <noggit/scripting/script_misc.hpp>

#include <noggit/scripting/script_chunk.hpp>
#include <noggit/scripting/script_model.hpp>
#include <noggit/scripting/script_vert.hpp>

#include <math/vector_3d.hpp>

using namespace das;
using namespace noggit::scripting;
using namespace math;

// TODO: Rename this one, it's the odd one
#define FUNC(name, side_effect) \
    addExtern<DAS_BIND_FUN(name)>(*this, lib, #name, SideEffects::side_effect, #name)

#define FUNC_RETVALUE(name, side_effect) \
    addExtern<DAS_BIND_FUN(noggit::scripting::name), SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, #name, SideEffects::side_effect, #name)

#define FUNC_SCOPED(name, side_effect) \
    addExtern<DAS_BIND_FUN(noggit::scripting::name)>(*this, lib, #name, SideEffects::side_effect, #name)

#define FIELD(name) \
    addField<DAS_BIND_MANAGED_FIELD(name)>(#name);

#define CLASS(name, ...)                                                             \
    MAKE_TYPE_FACTORY(name, name);                                                   \
    struct name##_annotation : public ManagedStructureAnnotation<name, true, true>   \
    {                                                                                \
        name##_annotation(ModuleLibrary &ml) : ManagedStructureAnnotation(#name, ml) \
        {                                                                            \
            __VA_ARGS__                                                              \
        }                                                                            \
        virtual bool isLocal() const override { return true; }                       \
        virtual bool canCopy() const override { return true; }                       \
        virtual bool canMove() const override { return true; }                       \
    };

#define CLASS_TEMPLATE(name, template, regname, ...)                                                       \
    MAKE_TYPE_FACTORY(name<template>, name<template>);                                                     \
    struct name##_##template##_annotation : public ManagedStructureAnnotation<name<template>, true, true>{ \
        name##_##template##_annotation(ModuleLibrary & ml) : ManagedStructureAnnotation(#regname, ml){     \
            __VA_ARGS__} virtual bool isLocal() const override{return true;                                \
    }                                                                                                      \
    virtual bool canCopy() const override { return true; }                                                 \
    virtual bool canMove() const override { return true; }                                                 \
    }                                                                                                      \
    ;

#define CLASS_ANNOTATION(name) \
    addAnnotation(make_smart<name##_annotation>(lib));

template <typename T>
struct test_class
{
};

// Classes
CLASS(vector_3d, FIELD(x) FIELD(y) FIELD(z))
CLASS(script_image)
CLASS(script_file_iterator)
CLASS(script_noise_map)
CLASS(script_noise_generator)
CLASS(script_chunk)
CLASS(script_vert)
CLASS(script_tex)
CLASS(script_model)
CLASS(script_model_iterator)

CLASS(script_random)
CLASS(script_selection)

CLASS_TEMPLATE(test_class, int, "test_class_int")

int *test_func()
{
    get_cur_tool()->addLog("Test function message!");
    return nullptr;
}

class NoggitModule : public Module
{
public:
    NoggitModule() : Module("noggit")
    {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();

        // class annotations
        CLASS_ANNOTATION(vector_3d);
        CLASS_ANNOTATION(script_image);
        CLASS_ANNOTATION(script_file_iterator);
        CLASS_ANNOTATION(script_noise_map);
        CLASS_ANNOTATION(script_noise_generator);
        CLASS_ANNOTATION(script_random);
        CLASS_ANNOTATION(script_selection);
        CLASS_ANNOTATION(script_chunk);
        CLASS_ANNOTATION(script_vert);
        CLASS_ANNOTATION(script_tex);
        CLASS_ANNOTATION(script_model);
        CLASS_ANNOTATION(script_model_iterator);

        // script_random.hpp
        FUNC_RETVALUE(random_from_seed, worstDefault);
        FUNC_RETVALUE(random_from_time, worstDefault);
        FUNC_SCOPED(rand_int32, worstDefault);
        FUNC_SCOPED(rand_uint32, worstDefault);
        FUNC_SCOPED(rand_double, worstDefault);
        FUNC_SCOPED(rand_float, worstDefault);

        // script_noise.hpp
        FUNC_SCOPED(noise_get_index, worstDefault);
        FUNC_SCOPED(noise_get_global, worstDefault);
        FUNC_SCOPED(noise_set, worstDefault);
        FUNC_SCOPED(noise_start_x, worstDefault);
        FUNC_SCOPED(noise_start_y, worstDefault);
        FUNC_SCOPED(noise_width, worstDefault);
        FUNC_SCOPED(noise_height, worstDefault);
        FUNC_SCOPED(noise_fill, worstDefault);
        FUNC_SCOPED(noise_fill_selection, worstDefault);
        FUNC_SCOPED(noise_is_highest_global, worstDefault);

        FUNC_RETVALUE(make_noisemap, worstDefault);
        FUNC_RETVALUE(make_noisegen_simplex, worstDefault);
        FUNC_RETVALUE(make_noisegen_perlin, worstDefault);
        FUNC_RETVALUE(make_noisegen_value, worstDefault);
        FUNC_RETVALUE(make_noisegen_fractal, worstDefault);
        FUNC_RETVALUE(make_noisegen_cellular, worstDefault);
        FUNC_RETVALUE(make_noisegen_white, worstDefault);
        FUNC_RETVALUE(make_noisegen_custom, worstDefault);

        // script_string.hpp
        FUNC_SCOPED(img_get_index, worstDefault);
        FUNC_SCOPED(img_get_pixel, worstDefault);
        FUNC_SCOPED(img_set_pixel, worstDefault);
        FUNC_SCOPED(img_gradient_scale, worstDefault);
        FUNC_SCOPED(img_save, worstDefault);
        FUNC_SCOPED(img_width, worstDefault);
        FUNC_SCOPED(img_height, worstDefault);
        FUNC_RETVALUE(create_image, worstDefault);
        FUNC_SCOPED(img_resize, worstDefault);
        FUNC_SCOPED(img_load_png, worstDefault);

        FUNC_SCOPED(file_itr_next, worstDefault);
        FUNC_SCOPED(file_itr_get, worstDefault);
        FUNC_SCOPED(write_file, worstDefault);
        FUNC_SCOPED(append_file, worstDefault);
        FUNC_RETVALUE(read_directory, worstDefault);
        FUNC_SCOPED(read_file, worstDefault);
        FUNC_SCOPED(path_exists, worstDefault);

        // selections.hpp
        FUNC_RETVALUE(make_selector, worstDefault);
        FUNC_SCOPED(select_origin, worstDefault);
        FUNC_SCOPED(select_between, worstDefault);
        FUNC_SCOPED(sel_next_chunk, worstDefault);
        FUNC_RETVALUE(sel_get_chunk, worstDefault);
        FUNC_SCOPED(sel_reset_chunk_itr, worstDefault);
        FUNC_SCOPED(sel_next_model, worstDefault);
        FUNC_RETVALUE(sel_get_model, worstDefault);
        FUNC_SCOPED(sel_reset_model_itr, worstDefault);
        FUNC_SCOPED(sel_requery_models, worstDefault);
        FUNC_RETVALUE(sel_center, worstDefault);
        FUNC_RETVALUE(sel_min, worstDefault);
        FUNC_RETVALUE(sel_max, worstDefault);
        FUNC_RETVALUE(sel_size, worstDefault);

        FUNC_SCOPED(chunk_set_hole, worstDefault);
        FUNC_SCOPED(chunk_remove_texture, worstDefault);
        FUNC_SCOPED(chunk_get_texture, worstDefault);
        FUNC_SCOPED(chunk_add_texture, worstDefault);
        FUNC_SCOPED(chunk_clear_textures, worstDefault);
        FUNC_SCOPED(chunk_apply_textures, worstDefault);
        FUNC_SCOPED(chunk_apply_heightmap, worstDefault);
        FUNC_SCOPED(chunk_apply_vertex_color, worstDefault);
        FUNC_SCOPED(chunk_apply_all, worstDefault);
        FUNC_SCOPED(chunk_set_impassable, worstDefault);
        FUNC_SCOPED(chunk_get_area_id, worstDefault);
        FUNC_SCOPED(chunk_set_area_id, worstDefault);
        FUNC_SCOPED(chunk_next_vert, worstDefault);
        FUNC_SCOPED(chunk_next_tex, worstDefault);
        FUNC_SCOPED(chunk_reset_vert_itr, worstDefault);
        FUNC_SCOPED(chunk_reset_tex_itr, worstDefault);
        FUNC_SCOPED(chunk_clear_colors, worstDefault);
        FUNC_RETVALUE(chunk_get_vert, worstDefault);
        FUNC_RETVALUE(chunk_get_tex, worstDefault);

        FUNC_RETVALUE(vert_get_pos, worstDefault);
        FUNC_SCOPED(vert_set_height, worstDefault);
        FUNC_SCOPED(vert_add_height, worstDefault);
        FUNC_SCOPED(vert_sub_height, worstDefault);
        FUNC_SCOPED(vert_set_color, worstDefault);
        FUNC_SCOPED(vert_set_water, worstDefault);
        FUNC_SCOPED(vert_set_hole, worstDefault);
        FUNC_SCOPED(vert_set_alpha, worstDefault);
        FUNC_SCOPED(vert_get_alpha, worstDefault);
        FUNC_SCOPED(vert_next_tex, worstDefault);
        FUNC_SCOPED(vert_reset_tex, worstDefault);
        FUNC_RETVALUE(vert_get_tex, worstDefault);
        FUNC_SCOPED(vert_is_water_aligned, worstDefault);
        FUNC_SCOPED(tex_set_alpha, worstDefault);
        FUNC_SCOPED(tex_get_alpha, worstDefault);
        FUNC_RETVALUE(tex_get_pos_2d, worstDefault);

        FUNC_RETVALUE(model_get_pos, worstDefault);
        FUNC_SCOPED(model_set_pos, worstDefault);
        FUNC_RETVALUE(model_get_rot, worstDefault);
        FUNC_SCOPED(model_set_rot, worstDefault);
        FUNC_SCOPED(model_get_scale, worstDefault);
        FUNC_SCOPED(model_set_scale, worstDefault);
        FUNC_SCOPED(model_get_uid, worstDefault);
        FUNC_SCOPED(model_remove, worstDefault);
        FUNC_SCOPED(model_get_filename, worstDefault);
        FUNC_SCOPED(model_set_filename, worstDefault);

        // script_math.hpp
        FUNC_SCOPED(round, worstDefault);
        FUNC_SCOPED(pow, worstDefault);
        FUNC_SCOPED(log10, worstDefault);
        FUNC_SCOPED(log, worstDefault);
        FUNC_SCOPED(ceil, worstDefault);
        FUNC_SCOPED(floor, worstDefault);
        FUNC_SCOPED(exp, worstDefault);
        FUNC_SCOPED(cbrt, worstDefault);
        FUNC_SCOPED(acosh, worstDefault);
        FUNC_SCOPED(asinh, worstDefault);
        FUNC_SCOPED(atanh, worstDefault);
        FUNC_SCOPED(cosh, worstDefault);
        FUNC_SCOPED(sinh, worstDefault);
        FUNC_SCOPED(tanh, worstDefault);
        FUNC_SCOPED(acos, worstDefault);
        FUNC_SCOPED(asin, worstDefault);
        FUNC_SCOPED(atan, worstDefault);
        FUNC_SCOPED(cos, worstDefault);
        FUNC_SCOPED(sin, worstDefault);
        FUNC_SCOPED(tan, worstDefault);
        FUNC_SCOPED(sqrt, worstDefault);
        FUNC_SCOPED(abs, worstDefault);
        FUNC_SCOPED(lerp, worstDefault);
        FUNC_SCOPED(dist_2d, worstDefault);
        FUNC_SCOPED(dist_2d_compare, worstDefault);
        FUNC_RETVALUE(rotate_2d, worstDefault);

        // script_context.hpp
        FUNC_RETVALUE(pos, worstDefault);
        FUNC_RETVALUE(vec, worstDefault);
        FUNC_SCOPED(brush_change_terrain, worstDefault);
        FUNC_SCOPED(add_m2, worstDefault);
        FUNC_SCOPED(add_wmo, worstDefault);
        FUNC_SCOPED(get_map_id, worstDefault);
        FUNC_SCOPED(get_area_id, worstDefault);
        FUNC_SCOPED(brush_set_area_id, worstDefault);
        FUNC_SCOPED(brush_change_vertex_color, worstDefault);
        FUNC_RETVALUE(brush_get_vertex_color, worstDefault);
        FUNC_SCOPED(brush_flatten_terrain, worstDefault);
        FUNC_SCOPED(brush_blur_terrain, worstDefault);
        FUNC_SCOPED(brush_erase_textures, worstDefault);
        FUNC_SCOPED(brush_clear_shadows, worstDefault);
        FUNC_SCOPED(brush_clear_textures, worstDefault);
        FUNC_SCOPED(brush_clear_height, worstDefault);
        FUNC_SCOPED(brush_set_hole, worstDefault);
        FUNC_SCOPED(brush_set_hole_adt, worstDefault);
        FUNC_SCOPED(brush_deselect_vertices, worstDefault);
        FUNC_SCOPED(brush_clear_vertex_selection, worstDefault);
        FUNC_SCOPED(brush_move_vertices, worstDefault);
        FUNC_SCOPED(brush_flatten_vertices, worstDefault);
        FUNC_SCOPED(brush_update_vertices, worstDefault);
        FUNC_SCOPED(brush_paint_texture, worstDefault);
        FUNC_SCOPED(cam_pitch, worstDefault);
        FUNC_SCOPED(cam_yaw, worstDefault);
        FUNC_SCOPED(outer_radius, worstDefault);
        FUNC_SCOPED(inner_radius, worstDefault);
        FUNC_SCOPED(holding_alt, worstDefault);
        FUNC_SCOPED(holding_shift, worstDefault);
        FUNC_SCOPED(holding_ctrl, worstDefault);
        FUNC_SCOPED(holding_space, worstDefault);

        // scripting_tool.hpp
        FUNC_SCOPED(get_string_param, worstDefault);
        FUNC_SCOPED(get_string_list_param, worstDefault);
        FUNC_SCOPED(get_int_param, worstDefault);
        FUNC_SCOPED(get_double_param, worstDefault);
        FUNC_SCOPED(get_bool_param, worstDefault);
        FUNC_SCOPED(add_string_param, worstDefault);
        FUNC_SCOPED(add_int_param, worstDefault);
        FUNC_SCOPED(add_double_param, worstDefault);
        FUNC_SCOPED(add_float_param, worstDefault);
        FUNC_SCOPED(get_float_param, worstDefault);
        FUNC_SCOPED(add_bool_param, worstDefault);
        FUNC_SCOPED(add_string_list_param, worstDefault);
        FUNC_SCOPED(add_description, worstDefault);
    }
};

REGISTER_MODULE(NoggitModule);