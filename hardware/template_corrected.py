import matplotlib.pyplot as plt
import numpy as np

# Set up the figure (A4 landscape in mm, converted to inches for matplotlib)
fig = plt.figure(figsize=(11.69, 8.27))  # 297 mm x 210 mm in inches
ax = fig.add_subplot(111)
ax.set_aspect('equal')

# Set limits in mm
ax.set_xlim(0, 297)
ax.set_ylim(0, 210)
ax.set_xlabel('Width (mm)')
ax.set_ylabel('Height (mm)')
ax.grid(True, alpha=0.3)

# CORRECTED dimensions from user specifications (in mm)
display_width, display_height = 29, 25.8  # TFT displays (landscape orientation)
square_button_size = 18  # Top surface of square button
square_mount_hole = 16  # Mounting hole for square button
round_button_diameter = 33  # Round select button diameter
mount_hole_size = 2  # Display mounting holes
wire_hole_size = 3  # Wire holes

# Layout parameters
horizontal_spacing = 16  # Space between display centres minus display width
vertical_spacing = 90  # Centre-to-centre spacing between rows
start_x = 30  # Left margin
row1_y = 40  # First row Y position
row2_y = row1_y + vertical_spacing  # Second row Y position

# Button and select button positions
button_offset_y = 40  # Distance below display for task buttons
select_button_x = 220  # X position for row select buttons

# Colors for different elements
display_color = 'blue'
mount_color = 'red'
wire_color = 'green'
button_color = 'purple'
select_color = 'orange'

# Draw all 8 displays in 2×4 grid
displays_per_row = 4
display_labels = ['D0', 'D1', 'D2', 'D3', 'D4', 'D5', 'D6', 'D7']
button_labels = ['B0', 'B1', 'B2', 'B3', 'B4', 'B5', 'B6', 'B7']

for i in range(8):
    # Calculate position for each display
    col = i % displays_per_row
    row = i // displays_per_row
    
    x = start_x + col * (display_width + horizontal_spacing)
    y = row1_y if row == 0 else row2_y
    
    # Draw display rectangle (29mm × 25.8mm in landscape)
    display_rect = plt.Rectangle((x, y), display_width, display_height, 
                                fill=False, edgecolor=display_color, linewidth=2)
    ax.add_patch(display_rect)
    
    # Add display label in centre
    ax.text(x + display_width/2, y + display_height/2, display_labels[i],
            ha='center', va='center', fontsize=10, fontweight='bold')
    
    # Draw mounting holes at corners (2mm diameter)
    corner_offset = 2
    corners = [(x + corner_offset, y + corner_offset),
               (x + display_width - corner_offset, y + corner_offset),
               (x + corner_offset, y + display_height - corner_offset),
               (x + display_width - corner_offset, y + display_height - corner_offset)]
    
    for corner_x, corner_y in corners:
        mount_hole = plt.Circle((corner_x, corner_y), mount_hole_size/2, 
                               fill=False, edgecolor=mount_color, linewidth=1)
        ax.add_patch(mount_hole)
    
    # Draw wire holes below display (3mm diameter, two holes)
    wire_y = y + display_height + 5
    wire1 = plt.Circle((x + display_width/2 - 1.5, wire_y), wire_hole_size/2,
                      fill=False, edgecolor=wire_color, linewidth=1.5)
    wire2 = plt.Circle((x + display_width/2 + 1.5, wire_y), wire_hole_size/2,
                      fill=False, edgecolor=wire_color, linewidth=1.5)
    ax.add_patch(wire1)
    ax.add_patch(wire2)
    
    # Draw task button below wire holes (18×18mm with 16mm mount hole)
    button_y = wire_y + 10
    button_x = x + (display_width - square_button_size)/2
    
    # Square button outline (18×18mm)
    button_rect = plt.Rectangle((button_x, button_y), square_button_size, square_button_size,
                               fill=False, edgecolor=button_color, linewidth=2)
    ax.add_patch(button_rect)
    
    # Button mounting hole (16mm diameter, dashed)
    mount_circle = plt.Circle((button_x + square_button_size/2, button_y + square_button_size/2), 
                             square_mount_hole/2, fill=False, edgecolor=button_color, 
                             linewidth=1, linestyle='--')
    ax.add_patch(mount_circle)
    
    # Add button label
    ax.text(button_x + square_button_size/2, button_y + square_button_size/2, button_labels[i],
            ha='center', va='center', fontsize=8)

# Draw row select buttons (33mm diameter)
select_positions = [(select_button_x, row1_y + display_height/2),
                   (select_button_x, row2_y + display_height/2)]
select_labels = ['R1', 'R2']

for i, (sx, sy) in enumerate(select_positions):
    # Select button circle (33mm diameter)
    select_circle = plt.Circle((sx, sy), round_button_diameter/2,
                              fill=False, edgecolor=select_color, linewidth=3)
    ax.add_patch(select_circle)
    
    # Mount hole indication (33mm diameter, dotted)
    mount_circle = plt.Circle((sx, sy), round_button_diameter/2,
                             fill=False, edgecolor=select_color, linewidth=1, linestyle=':')
    ax.add_patch(mount_circle)
    
    # Row select label
    ax.text(sx, sy, select_labels[i], ha='center', va='center', 
            fontsize=12, fontweight='bold')

# Add dimension annotations
def add_dimension_line(x1, y1, x2, y2, offset, text, text_rotation=0):
    """Add a dimension line with text"""
    ax.plot([x1, x2], [y1, y2], 'k-', linewidth=0.5, alpha=0.7)
    ax.plot([x1, x1], [y1-offset/2, y1+offset/2], 'k-', linewidth=0.5, alpha=0.7)
    ax.plot([x2, x2], [y2-offset/2, y2+offset/2], 'k-', linewidth=0.5, alpha=0.7)
    mid_x, mid_y = (x1+x2)/2, (y1+y2)/2
    ax.text(mid_x, mid_y-offset, text, ha='center', va='top', fontsize=8, 
            rotation=text_rotation, color='gray')

# Key dimensions
# Display width
add_dimension_line(start_x, row1_y-10, start_x+display_width, row1_y-10, 3, f'{display_width}mm')

# Display height  
add_dimension_line(start_x-15, row1_y, start_x-15, row1_y+display_height, 3, f'{display_height}mm', 90)

# Spacing between displays
add_dimension_line(start_x+display_width, row1_y-5, start_x+display_width+horizontal_spacing, row1_y-5, 2, f'{horizontal_spacing}mm')

# Row spacing
add_dimension_line(start_x-25, row1_y+display_height/2, start_x-25, row2_y+display_height/2, 3, f'{vertical_spacing}mm', 90)

# Title
plt.title('ChoreTracker A4 Template - 2×4 Display Grid\n(Corrected: 29×25.8mm Landscape Displays)', 
          fontsize=14, fontweight='bold', pad=20)

# Legend
legend_x, legend_y = 250, 180
legend_items = [
    (display_color, 'TFT Display (29×25.8mm)', 'rect'),
    (mount_color, 'Mount Holes (Ø2mm)', 'circle'),
    (wire_color, 'Wire Holes (Ø3mm)', 'circle'),
    (button_color, 'Task Buttons (18×18mm)', 'rect'),
    (button_color, '16mm Mount Hole', 'dashed_circle'),
    (select_color, 'Select Buttons (Ø33mm)', 'circle')
]

ax.text(legend_x, legend_y, 'Legend:', fontsize=10, fontweight='bold')
for i, item in enumerate(legend_items):
    y_pos = legend_y - 15 - i*12
    if len(item) == 3:
        color, text, shape = item
        if shape == 'rect':
            legend_rect = plt.Rectangle((legend_x, y_pos-3), 8, 6, fill=False, edgecolor=color, linewidth=2)
            ax.add_patch(legend_rect)
        elif shape == 'circle':
            legend_circle = plt.Circle((legend_x+4, y_pos), 3, fill=False, edgecolor=color, linewidth=2)
            ax.add_patch(legend_circle)
        elif shape == 'dashed_circle':
            legend_circle = plt.Circle((legend_x+4, y_pos), 3, fill=False, edgecolor=color, linewidth=1, linestyle='--')
            ax.add_patch(legend_circle)
        ax.text(legend_x+15, y_pos, text, fontsize=8, va='center')

# Add print instructions
ax.text(10, 10, 'Print at 100% scale on A4 landscape. Verify: TFT displays should measure exactly 29×25.8mm',
        fontsize=9, style='italic', bbox=dict(boxstyle="round,pad=0.3", facecolor="lightyellow"))

# Save files
plt.tight_layout()
plt.savefig('template_design_corrected.png', dpi=300, bbox_inches='tight', facecolor='white')
plt.savefig('choretracker_template_corrected.svg', format='svg', bbox_inches='tight', facecolor='white')
plt.show()

# Print summary
print("✅ Corrected template generated with accurate dimensions:")
print(f"   • TFT Displays: {display_width}×{display_height}mm (landscape orientation)")
print(f"   • Task Buttons: {square_button_size}×{square_button_size}mm surface, Ø{square_mount_hole}mm mount hole")
print(f"   • Select Buttons: Ø{round_button_diameter}mm (33mm mount hole)")
print(f"   • Display spacing: {horizontal_spacing}mm gap ({display_width + horizontal_spacing}mm centre-to-centre)")
print(f"   • Row spacing: {vertical_spacing}mm centre-to-centre")
print(f"   • Mount holes: Ø{mount_hole_size}mm (display corners)")
print(f"   • Wire holes: Ø{wire_hole_size}mm (two per display)")
print("   • Files saved: template_design_corrected.png and choretracker_template_corrected.svg") 