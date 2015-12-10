-- Mike Lattanzio's Level!

myroot = gr.node('root') -- The root node of the pupppet

--myroot:translate(0, 0, -25.0)

-- Material                         COLOUR__________  LIGHTING________  SHININESS
_wall     = gr.texturedmaterial({ 0.4, 0.4, 0.5}, { 0.2, 0.2, 0.2}, 60,  1)
_wall_map = gr.texturedmaterial({ 1.0, 1.0, 1.0}, { 0.0, 0.0, 0.0}, 100, 16)
_door     = gr.texturedmaterial({ 0.4, 0.4, 0.4}, { 0.7, 0.7, 0.7}, 20, 14)
_ceiling  = gr.texturedmaterial({ 0.3, 0.3, 0.3}, { 0.2, 0.2, 0.2}, 40, 7)
_floor    = gr.texturedmaterial({ 0.05, 0.05, 0.05}, { 0.1, 0.1, 0.1}, 100, 6)
_box      = gr.texturedmaterial({ 0.5, 0.5, 0.5}, { 0.2, 0.2, 0.2}, 40, 13)
_wood     = gr.texturedmaterial({ 0.4, 0.4, 0.2}, { 0.1, 0.1, 0.1}, 40, 15)


-- PROJECTION VARIABLES
ROOM_TILT = -75
ROOM_ZOOM = .60

-- Variables to simplify drawing
room_x = 16 -- Width of room
room_z = 12 -- Depth of room

room_y = 4  -- Height of ceiling

xjoint = {0,0,0}
yjoint = {0,0,0}


-- THE FLOOR, THE CENTRE OF THE FLOOR IS (0,0,0)
-- centre
centre = gr.node('floor_centre', xjoint, yjoint)
myroot:add_child( centre )

-- ROTATE THE SCENE
-- centre:rotate('x', ROOM_TILT)
-- centre:scale( ROOM_ZOOM, ROOM_ZOOM, ROOM_ZOOM)

-- Back
room_back = gr.node('room_back', xjoint, yjoint)
centre:add_child( room_back )
room_back:translate( 0, 0, -room_z )

-- Front
room_front = gr.node('room_front', xjoint, yjoint)
centre:add_child( room_front )
room_front:translate( 0, 0, room_z )

-- Right
room_right = gr.node('room_right', xjoint, yjoint)
centre:add_child( room_right )
room_right:translate( room_x, 0, 0 )

-- Left
room_left = gr.node('room_left', xjoint, yjoint)
centre:add_child( room_left )
room_left:translate( -room_x, 0, 0 )

-- DOOR JOINTS
door_right = gr.joint('door_right', xjoint, yjoint)
room_right:add_child( door_right )
door_left = gr.joint('door_left', xjoint, yjoint)
room_left:add_child( door_left )
door_front = gr.joint('door_front', xjoint, yjoint)
room_front:add_child( door_front )
door_back = gr.joint('door_back', xjoint, yjoint)
door_back:rotate( 'y', 180 )
room_back:add_child( door_back )

--
-- GEOMETRY NODES!

-- Floor
for i = -(room_x -1), (room_x - 1), 2 do
    for j = -(room_z - 1), (room_z - 1), 2 do
        floor = gr.plane('floor' .. i .. '-' .. j)
        centre:add_child( floor )
        floor:set_material( _floor )
        floor:translate( i, 0, j )
    end
end

for i = -room_x - 6, room_x + 6, 4 do

wall_back = 1
wall_front = 1

if math.abs(i) ~= 2 then -- this is a wall

wall_back = gr.plane('wall_back')
room_back:add_child( wall_back )
wall_front = gr.plane('wall_front')
room_front:add_child( wall_front )
wall_back:set_material( _wall )
wall_back:set_map( _wall_map )
wall_front:set_material( _wall )
wall_front:set_map( _wall_map )

	if math.abs(i) > room_x then

	wall_back:translate( 0, 0, room_z - 4 )
	wall_front:translate( 0, 0, -room_z + 4 )

	end

else -- this is a door

wall_back = gr.cube('door_back_geo')
door_back:add_child( wall_back )
wall_front = gr.cube('door_front_geo')
door_front:add_child( wall_front )
wall_back:set_material( _door )
wall_front:set_material( _door )

wall_back:scale( 1, .99, .1)
wall_front:scale( 1, .99, .1)

end

-- Back wall
wall_back:translate( i, 0, 0 )
wall_back:scale( 2, room_y / 2, 1 )
wall_back:translate( 0, 1, 0 )
wall_back:rotate( 'x', -90 )

-- Front Wall
wall_front:translate( i, 0, 0 )
wall_front:scale( 2, room_y / 2, 1 )
wall_front:translate( 0, 1, 0 )
wall_front:rotate( 'x', -90 )


end

for i = -room_z - 10, room_z + 10, 4 do

if math.abs(i) ~= 2 then -- this is a wall

wall_right = gr.plane('wall_right')
room_right:add_child( wall_right )
wall_left = gr.plane('wall_left')
room_left:add_child( wall_left )
wall_right:set_material( _wall )
wall_right:set_map( _wall_map )
wall_left:set_material( _wall )
wall_left:set_map( _wall_map )

	if math.abs(i) > room_z then

	wall_left:translate( room_x - 4 , 0, 0 )
	wall_right:translate( -room_x + 4, 0, 0 )

	end

else -- this is a door

wall_right = gr.cube('door_right_geo')
door_right:add_child( wall_right )
wall_left = gr.cube('door_left_geo')
door_left:add_child( wall_left )
wall_right:set_material( _door )
wall_left:set_material( _door )

wall_left:scale( .1, .99, 1)
wall_right:scale( .1, .99, 1)

end

-- Right Wall
wall_right:rotate( 'y', -90 )
wall_right:translate( i, 0, 0 )
wall_right:scale( 2, room_y / 2, 1 )
wall_right:translate( 0, 1, 0 )
wall_right:rotate( 'x', -90 )

-- Left Wall
wall_left:rotate( 'y', 90 )
wall_left:translate( i, 0, 0 )
wall_left:scale( 2, room_y / 2, 1 )
wall_left:translate( 0, 1, 0 )
wall_left:rotate( 'x', -90 )

end

-- Left And Right Ceiling
for i = (room_x + 1), 23, 2 do
    for j = -(room_z-1), (room_z-1), 2 do
        ceiling1 = gr.plane('ceiling' .. i .. '-' .. j)
        centre:add_child( ceiling1 )
        ceiling2 = gr.plane('ceiling' .. -i .. '-' .. j)
        centre:add_child( ceiling2 )
		 if math.abs( j ) > 3 then
        ceiling1:set_material( _ceiling )
        ceiling1:translate( i, room_y, j )
        ceiling2:set_material( _ceiling )
        ceiling2:translate( -i, room_y, j )
		 else
        ceiling1:set_material( _floor )
        ceiling1:translate( i, 0, j )
        ceiling2:set_material( _floor )
        ceiling2:translate( -i, 0, j )
		 end
    end
end

-- Top, Bottom, and corner Ceiling
for i = -23, 23, 2 do
    for j = (room_z +1), 23, 2 do

		  -- Create the tiles
		  ceiling1 = gr.plane('ceiling' .. i .. '-' .. j)
        centre:add_child( ceiling1 )
        ceiling2 = gr.plane('ceiling' .. i .. '-' .. -j)
        centre:add_child( ceiling2 )
		 if math.abs( i ) > 3 then
        ceiling1:set_material( _ceiling )
        ceiling1:translate( i, room_y, j )
        ceiling2:set_material( _ceiling )
        ceiling2:translate( i, room_y, -j )
   	 else
        ceiling1:set_material( _floor )
        ceiling1:translate( i, 0, j )
        ceiling2:set_material( _floor )
        ceiling2:translate( i, 0, -j )
		 end
    end
end
-- BOX
math.randomseed(os.time())

boxlayout = math.random(1, 4)

if boxlayout == 1 then

bigboxes = { {-14, 2, -10}, {-14, 2, -6}, {6, 2, 6}, {10, 2, 6} }
littleboxes = { {-11, 1,-11, 0},
          {-10.8, 3,-10.5, -14},
          {-11, 1, -9, 0},
          {-9, 1, -11, 0},
          {5, 1, 9,0},
          {6, 3, 9,0},
          {7, 1, 9,0},
          {9, 1, 9,0} }



elseif boxlayout == 2 then

bigboxes = { {3, 2, 2},  {3, 2, -2} }
littleboxes = { {3, 5, 3}, { -12, 1, 8}, {-10, 1, 8}, {-8, 1, 8} ,
                           { -12, 1, 6}, {-10, 1, 6}, {-8, 1, 6} , { -11, 3, 7.5}, {-9, 3, 7.7} }

elseif boxlayout == 3 then

bigboxes = { {-14, 2, -10},
             {-10, 2, -10},
             {-6, 2, -10},
             {6, 2, -10},
             {10, 2, -10},
             {14, 2, -10} }

littleboxes = { {-14, 5, -11},
                {-12, 5, -11},
                {-6, 5, -11},
                {8, 5, -11},
                {10, 5, -11},
                {12, 5, -11} }

elseif boxlayout == 4 then

bigboxes = { { -8, 2, -6}, {8, 2, 6} }
littleboxes = { { -9, 1, 7 }, {-7, 1, 7 }, {-9, 1, 5}, {-7, 1, 5 }, {-8.5, 3, 6.5} ,
                { 9, 1, -7 }, {7, 1, -7 }, {9, 1, -5}, {7, 1, -5 }, {7, 3, -6.2}, {9, 3, -6.7} }

end

for v in pairs(bigboxes) do
    box = gr.cube('box')
    centre:add_child( box )
    box:set_material( _box )
    box:translate( bigboxes[v][1], bigboxes[v][2] + .005, bigboxes[v][3] )
    box:scale( 2, 2, 2 )
    centre:add_child( box )
end
for v in pairs(littleboxes) do
    box = gr.cube('box_small')
    centre:add_child( box )
    box:set_material( _wood )
    box:translate( littleboxes[v][1], littleboxes[v][2] + .005, littleboxes[v][3] )
    --box:rotate( 'y', littleboxes[v][4] );
    centre:add_child( box )
end

return myroot
