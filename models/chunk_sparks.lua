-- Mike Lattanzio's Little Chunk

-- Material         COLOUR__________  LIGHTING________  SHININESS
spark = gr.material({ 0.7, 0.4, 0.0}, { 0.6, 0.6, 0.0}, 1)

xjoint = {1,2,3};
yjoint = {1,2,3};

myroot = gr.node('root')

particle = gr.cube('particle') 
particle:set_material( spark )
particle:scale( 0.2, 0.3, 0.1 ) 
myroot:add_child( particle )

return myroot
