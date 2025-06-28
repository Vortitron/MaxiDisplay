import matplotlib.pyplot as plt
import numpy as np

# Set up the figure (A4 landscape in mm, converted to inches for matplotlib)
fig = plt.figure(figsize=(11.69, 8.27))  # 297 mm x 210 mm in inches (1 mm = 0.03937 inches)
ax = fig.add_subplot(111)
ax.set_aspect('equal')

# Set limits in mm
ax.set_xlim(0, 297)
ax.set_ylim(0, 210)
ax.set_xlabel('Width (mm)')
ax.set_ylabel('Height (mm)')
ax.grid(True)

# Dimensions (in mm)
screen_width, screen_height = 29, 25.8
square_button_width, square_button_height = 18, 30
round_button_diameter = 33
hole_size = 2  # Wire hole size
mount_hole_size = 16  # Mounting hole for square button

# Spacing
vertical_spacing = 40  # Space between rows
horizontal_spacing = 10  # Space between components

# Row 1 positions
screen1_x, screen1_y = 10, 10
square1_x, square1_y = screen1_x, screen1_y + screen_height + 10
round1_x, round1_y = screen1_x + screen_width + horizontal_spacing, screen1_y

# Row 2 positions
screen2_x, screen2_y = 10, screen1_y + vertical_spacing
square2_x, square2_y = screen2_x, screen2_y + screen_height + 10
round2_x, round2_y = screen2_x + screen_width + horizontal_spacing, screen2_y

# Draw screens
ax.add_patch(plt.Rectangle((screen1_x, screen1_y), screen_width, screen_height, fill=False, edgecolor='b'))
ax.add_patch(plt.Rectangle((screen2_x, screen2_y), screen_width, screen_height, fill=False, edgecolor='b'))

# Draw square buttons
ax.add_patch(plt.Rectangle((square1_x, square1_y), square_button_width, square_button_height, fill=False, edgecolor='r'))
ax.add_patch(plt.Rectangle((square2_x, square2_y), square_button_width, square_button_height, fill=False, edgecolor='r'))

# Draw round buttons
circle1 = plt.Circle((round1_x + round_button_diameter/2, round1_y + round_button_diameter/2), round_button_diameter/2, fill=False, edgecolor='g')
circle2 = plt.Circle((round2_x + round_button_diameter/2, round2_y + round_button_diameter/2), round_button_diameter/2, fill=False, edgecolor='g')
ax.add_patch(circle1)
ax.add_patch(circle2)

# Draw wire holes for screens (bottom center)
wire_hole1_x = screen1_x + screen_width/2
wire_hole1_y = screen1_y + screen_height
ax.add_patch(plt.Circle((wire_hole1_x, wire_hole1_y), hole_size/2, fill=False, edgecolor='k'))
ax.add_patch(plt.Circle((wire_hole1_x + 5, wire_hole1_y), hole_size/2, fill=False, edgecolor='k'))  # Second hole

wire_hole2_x = screen2_x + screen_width/2
wire_hole2_y = screen2_y + screen_height
ax.add_patch(plt.Circle((wire_hole2_x, wire_hole2_y), hole_size/2, fill=False, edgecolor='k'))
ax.add_patch(plt.Circle((wire_hole2_x + 5, wire_hole2_y), hole_size/2, fill=False, edgecolor='k'))  # Second hole

# Draw mounting holes for square buttons (center)
mount1_x = square1_x + square_button_width/2
mount1_y = square1_y + square_button_height/2
ax.add_patch(plt.Circle((mount1_x, mount1_y), mount_hole_size/2, fill=False, edgecolor='k', linestyle='--'))

mount2_x = square2_x + square_button_width/2
mount2_y = square2_y + square_button_height/2
ax.add_patch(plt.Circle((mount2_x, mount2_y), mount_hole_size/2, fill=False, edgecolor='k', linestyle='--'))

# Title and save
plt.title('A4 Template Design (297 mm x 210 mm)')
plt.savefig('template_design.png', dpi=300, bbox_inches='tight')
plt.show()