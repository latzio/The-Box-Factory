-- Mike Lattanzio's Little Chunk

-- Material         COLOUR__________  LIGHTING________  SHININESS
chunk = gr.texturedmaterial({ 0.35, 0.25, 0.075}, { 0.2, 0.2, 0.2}, 10, 10)

xjoint = {1,2,3};
yjoint = {1,2,3};

myroot = gr.node('root')

particle = gr.cube('particle') 
particle:set_material( chunk )
myroot:add_child( particle )

return myroot
