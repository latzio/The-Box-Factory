-- Mike Lattanzio's Little Chunk

-- Material         COLOUR__________  LIGHTING________  SHININESS
chunk = gr.material({ 0.5, 0.0, 0.0}, { 0.2, 0.2, 0.2}, 10)

xjoint = {1,2,3};
yjoint = {1,2,3};

myroot = gr.node('root')

cog = gr.joint('cog', xjoint, yjoint)
myroot:add_child( cog )

particle = gr.cube('particle') 
particle:set_material( chunk )
particle:translate( 4, 1, 0 )
cog:add_child( particle )

particle = gr.cube('particle') 
particle:set_material( chunk )
particle:translate( -4, -1, 0 )
cog:add_child( particle )

particle = gr.cube('particle') 
particle:set_material( chunk )
particle:translate( 0, 0, 4 )
cog:add_child( particle )

particle = gr.cube('particle') 
particle:set_material( chunk )
particle:translate( 0, 0, -4 )
cog:add_child( particle )

return myroot
