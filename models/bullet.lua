-- Mike Lattanzio's Bullet

shell = gr.texturedmaterial({ 0.9, 0.9, 0.9}, { 0.8, 0.8, 0.8}, 20, 10)

xjoint = {1,1,1};
yjoint = {1,1,1};

-- Size and shape
caliber = 0.2
length =  0.5

myroot = gr.node('root')

cog = gr.joint('cog', xjoint, yjoint)
myroot:add_child( cog )

travel = gr.node('travel')
cog:add_child(travel)

bullet = gr.cube('shell', xjoint, yjoint) -- The rotation node (centre of gravity)
bullet:set_material( shell )
bullet:scale( caliber, caliber, length )
travel:add_child( bullet )

return myroot

