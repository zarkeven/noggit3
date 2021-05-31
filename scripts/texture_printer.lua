local texture_printer = brush("Texture Printer");

texture_printer:add_description("<b>Description</b>: Prints out texture paths and effect ids in the clicked chunk")
texture_printer:add_null_tag()

function texture_printer:on_left_click(evt)
    local sel = select_origin(evt:pos(), 1, 1)
    local chunks = sel:chunks()
    while chunks:next() do
        local chunk = chunks:get()
        
        if chunk:get_texture_count() == 0 then
            print("Chunk has no textures")
        end

        print("== Chunk Textures ==")
        for i=0,chunk:get_texture_count()-1 do
            local tex = chunk:get_texture(i)
            local eff = chunk:get_effect(i)
            print("Layer "..i..":")
            print("    Texture: "..tex)
            print("    Effect: "..eff)
        end
        print("")
    end
end