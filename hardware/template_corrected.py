#!/usr/bin/env python3
"""
ChoreTracker A4 Template Generator (Corrected Dimensions)
Generates both PNG and SVG templates for physical construction.

Corrected dimensions based on user feedback:
- TFT Display: 57.9mm × 29.0mm 
- Mount holes: 1.5mm from edge  
- Square buttons: 16mm hole
- Round buttons: 29mm hole
- Displays moved left for button clearance
- Pin holes on left side for back connectors
"""

import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.patches import Rectangle, Circle
import numpy as np

# A4 dimensions in mm
A4_WIDTH = 297
A4_HEIGHT = 210

# Corrected component dimensions
DISPLAY_WIDTH = 57.9    # Corrected from 29mm 
DISPLAY_HEIGHT = 29.0   # Corrected from 25.8mm

# Button dimensions  
SQUARE_BUTTON_SIZE = 18     # 18×18mm surface
SQUARE_BUTTON_HOLE = 16     # 16mm mount hole
ROUND_BUTTON_HOLE = 29      # 29mm mount hole

# Layout parameters - REDESIGNED COMPACT LAYOUT
DISPLAYS_PER_ROW = 4
ROWS = 2
DISPLAY_SPACING = 5         # Gap between displays

# Redesigned vertical positions: B-D-S-D-B
BUTTONS_ROW1_Y = 25      # Top buttons row
DISPLAYS_ROW1_Y = 50     # First display row
SELECT_BUTTON1_Y = 100   # R1 select button (lower than before)
SELECT_BUTTON2_Y = 108   # R2 select button (slightly lower than R1 to show it goes with lower screens)
DISPLAYS_ROW2_Y = 125    # Second display row (moved down)
BUTTONS_ROW2_Y = 165     # Bottom buttons row (moved down)

# Margins  
MARGIN_LEFT = 15
MARGIN_TOP = 30

def create_template(show_legend=True):
    """Create the ChoreTracker template with corrected dimensions."""
    
    # Create figure with A4 dimensions
    fig, ax = plt.subplots(1, 1, figsize=(A4_WIDTH/25.4, A4_HEIGHT/25.4))
    ax.set_xlim(0, A4_WIDTH)
    ax.set_ylim(0, A4_HEIGHT)
    ax.set_aspect('equal')
    ax.invert_yaxis()  # Flip Y axis to match typical page layout
    
    # Title
    ax.text(A4_WIDTH/2, 15, 'ChoreTracker A4 Template - REDESIGNED COMPACT LAYOUT', 
            ha='center', va='center', fontsize=14, fontweight='bold')
    ax.text(A4_WIDTH/2, 25, 'Layout: Buttons-Displays-Select-Displays-Buttons (B-D-S-D-B)', 
            ha='center', va='center', fontsize=10)
    
    # Calculate total width needed
    total_display_width = DISPLAYS_PER_ROW * DISPLAY_WIDTH
    total_spacing = (DISPLAYS_PER_ROW - 1) * DISPLAY_SPACING
    total_width = total_display_width + total_spacing
    
    print(f"Layout calculations:")
    print(f"Display width: {DISPLAY_WIDTH}mm")
    print(f"Total display width: {total_display_width}mm")
    print(f"Total spacing: {total_spacing}mm")
    print(f"Total layout width: {total_width}mm")
    print(f"Available A4 width: {A4_WIDTH}mm")
    print(f"Remaining margin space: {A4_WIDTH - total_width - 2*MARGIN_LEFT}mm")
    
    # Draw displays and components - REDESIGNED LAYOUT
    for row in range(ROWS):
        for col in range(DISPLAYS_PER_ROW):
            display_num = row * DISPLAYS_PER_ROW + col
            
            # Calculate position with new layout
            x = MARGIN_LEFT + col * (DISPLAY_WIDTH + DISPLAY_SPACING)
            # Use new fixed positions for each row
            if row == 0:
                y = DISPLAYS_ROW1_Y
            else:
                y = DISPLAYS_ROW2_Y
            
            # Draw TFT display
            display_rect = Rectangle((x, y), DISPLAY_WIDTH, DISPLAY_HEIGHT,
                                   linewidth=2, edgecolor='blue', facecolor='lightblue', alpha=0.7)
            ax.add_patch(display_rect)
            
            # Display label
            ax.text(x + DISPLAY_WIDTH/2, y + DISPLAY_HEIGHT/2, f'D{display_num}',
                   ha='center', va='center', fontweight='bold', fontsize=10)
            
            # Mounting holes (1.5mm from edges)
            hole_inset = 1.5
            hole_positions = [
                (x + hole_inset, y + hole_inset),                                    # Top-left
                (x + DISPLAY_WIDTH - hole_inset, y + hole_inset),                   # Top-right  
                (x + hole_inset, y + DISPLAY_HEIGHT - hole_inset),                 # Bottom-left
                (x + DISPLAY_WIDTH - hole_inset, y + DISPLAY_HEIGHT - hole_inset)  # Bottom-right
            ]
            
            for hx, hy in hole_positions:
                hole = Circle((hx, hy), 1, linewidth=1, edgecolor='red', facecolor='white')
                ax.add_patch(hole)
            
            # Wire hole (inside display, 1mm from right edge, 27mm high)
            wire_hole_x = x + DISPLAY_WIDTH - 1  # 1mm from right edge
            wire_hole_y = y + 1  # 1mm from top edge
            wire_hole_width = 2  # 2mm wide slot
            wire_hole_height = 27  # 27mm high
            
            # Draw wire hole as a rectangle
            wire_hole = Rectangle((wire_hole_x - wire_hole_width/2, wire_hole_y), 
                                wire_hole_width, wire_hole_height,
                                linewidth=2, edgecolor='green', facecolor='lightgreen', alpha=0.5)
            ax.add_patch(wire_hole)
            
            # Task button (square button with round mount hole) - NEW POSITIONS
            button_x = x + DISPLAY_WIDTH/2 - SQUARE_BUTTON_SIZE/2
            # Buttons now at fixed positions: row 1 buttons above, row 2 buttons below
            if row == 0:
                button_y = BUTTONS_ROW1_Y
            else:
                button_y = BUTTONS_ROW2_Y
            
            # Button surface (18×18mm)
            button_rect = Rectangle((button_x, button_y), SQUARE_BUTTON_SIZE, SQUARE_BUTTON_SIZE,
                                  linewidth=2, edgecolor='purple', facecolor='white')
            ax.add_patch(button_rect)
            
            # Mount hole (16mm diameter)
            button_center_x = x + DISPLAY_WIDTH/2
            button_center_y = button_y + SQUARE_BUTTON_SIZE/2
            mount_hole = Circle((button_center_x, button_center_y), SQUARE_BUTTON_HOLE/2,
                              linewidth=1, edgecolor='purple', facecolor='none', linestyle='--')
            ax.add_patch(mount_hole)
            
            # Button label
            ax.text(button_center_x, button_center_y, f'B{display_num}',
                   ha='center', va='center', fontsize=8)
            
            # Add dimensions for first display
            if display_num == 0:
                # Display width dimension
                add_dimension_line(ax, x, y-5, x+DISPLAY_WIDTH, y-5, f'{DISPLAY_WIDTH}mm')
                # Display height dimension  
                add_dimension_line(ax, x-10, y, x-10, y+DISPLAY_HEIGHT, f'{DISPLAY_HEIGHT}mm', rotation=90)
    
    # Row select buttons - REPOSITIONED BETWEEN DISPLAY ROWS
    # Position at screen 1 (column 0) and screen 4 (column 3) positions
    select_button1_x = MARGIN_LEFT + DISPLAY_WIDTH/2  # Screen 1 position 
    select_button2_x = MARGIN_LEFT + 3 * (DISPLAY_WIDTH + DISPLAY_SPACING) + DISPLAY_WIDTH/2  # Screen 4 position
    
    # Row 1 select button (at screen 1 position)
    select_button = Circle((select_button1_x, SELECT_BUTTON1_Y), ROUND_BUTTON_HOLE/2,
                         linewidth=3, edgecolor='orange', facecolor='white')
    ax.add_patch(select_button)
    
    mount_outline = Circle((select_button1_x, SELECT_BUTTON1_Y), ROUND_BUTTON_HOLE/2,
                         linewidth=1, edgecolor='orange', facecolor='none', linestyle='--')
    ax.add_patch(mount_outline)
    
    ax.text(select_button1_x, SELECT_BUTTON1_Y, 'R1',
           ha='center', va='center', fontweight='bold', fontsize=10)
    ax.text(select_button1_x, SELECT_BUTTON1_Y + ROUND_BUTTON_HOLE/2 + 8, f'{ROUND_BUTTON_HOLE}mm hole',
           ha='center', va='center', fontsize=8)
    
    # Row 2 select button (at screen 4 position)
    select_button = Circle((select_button2_x, SELECT_BUTTON2_Y), ROUND_BUTTON_HOLE/2,
                         linewidth=3, edgecolor='orange', facecolor='white')
    ax.add_patch(select_button)
    
    mount_outline = Circle((select_button2_x, SELECT_BUTTON2_Y), ROUND_BUTTON_HOLE/2,
                         linewidth=1, edgecolor='orange', facecolor='none', linestyle='--')
    ax.add_patch(mount_outline)
    
    ax.text(select_button2_x, SELECT_BUTTON2_Y, 'R2',
           ha='center', va='center', fontweight='bold', fontsize=10)
    ax.text(select_button2_x, SELECT_BUTTON2_Y + ROUND_BUTTON_HOLE/2 + 8, f'{ROUND_BUTTON_HOLE}mm hole',
           ha='center', va='center', fontsize=8)
    
    # Legend (optional)
    if show_legend:
        legend_x = 15
        legend_y = 105
        
        ax.text(legend_x, legend_y, 'Legend:', fontweight='bold', fontsize=12)
        
        # TFT Display legend
        legend_rect = Rectangle((legend_x, legend_y+8), 25, 8, 
                              linewidth=2, edgecolor='blue', facecolor='lightblue', alpha=0.7)
        ax.add_patch(legend_rect)
        ax.text(legend_x + 30, legend_y + 12, f'TFT Display ({DISPLAY_WIDTH}×{DISPLAY_HEIGHT}mm)', fontsize=9)
        
        # Mount hole legend
        legend_hole = Circle((legend_x + 12, legend_y + 25), 1, 
                            linewidth=1, edgecolor='red', facecolor='white')
        ax.add_patch(legend_hole)
        ax.text(legend_x + 30, legend_y + 25, 'Mount Hole (Ø2mm, 1.5mm from edge)', fontsize=9)
        
        # Wire hole legend (updated)
        wire_legend = Rectangle((legend_x + 10, legend_y + 35), 2, 8, 
                               linewidth=2, edgecolor='green', facecolor='lightgreen', alpha=0.5)
        ax.add_patch(wire_legend)
        ax.text(legend_x + 30, legend_y + 38, 'Wire Hole (2×27mm, 1mm from right edge)', fontsize=9)
        
        # Square button legend
        button_legend_rect = Rectangle((legend_x + 7, legend_y + 47), 10, 10,
                                     linewidth=2, edgecolor='purple', facecolor='white')
        button_legend_hole = Circle((legend_x + 12, legend_y + 52), 5,
                                  linewidth=1, edgecolor='purple', facecolor='none', linestyle='--')
        ax.add_patch(button_legend_rect)
        ax.add_patch(button_legend_hole)
        ax.text(legend_x + 30, legend_y + 52, f'Task Button 18×18mm ({SQUARE_BUTTON_HOLE}mm mount hole)', fontsize=9)
        
        # Round button legend
        round_legend = Circle((legend_x + 12, legend_y + 68), 7,
                             linewidth=2, edgecolor='orange', facecolor='white')
        round_legend_hole = Circle((legend_x + 12, legend_y + 68), 7,
                                 linewidth=1, edgecolor='orange', facecolor='none', linestyle='--')
        ax.add_patch(round_legend)
        ax.add_patch(round_legend_hole)
        ax.text(legend_x + 30, legend_y + 68, f'Select Button Ø{ROUND_BUTTON_HOLE}mm ({ROUND_BUTTON_HOLE}mm mount hole)', fontsize=9)
    
    # Print instructions
    ax.text(10, A4_HEIGHT - 5, 
           'Print at 100% scale on A4 landscape. UPDATED: Bottom row moved down, wire holes 2×27mm @ 1mm from right. Verify: displays = 57.9×29.0mm',
           fontsize=10, style='italic')
    
    # Remove axes
    ax.set_xticks([])
    ax.set_yticks([])
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['bottom'].set_visible(False)
    ax.spines['left'].set_visible(False)
    
    return fig, ax

def add_dimension_line(ax, x1, y1, x2, y2, text, rotation=0):
    """Add a dimension line with text."""
    # Draw the line
    ax.plot([x1, x2], [y1, y2], color='gray', linewidth=1, linestyle='--')
    
    # Add dimension text
    mid_x = (x1 + x2) / 2
    mid_y = (y1 + y2) / 2
    ax.text(mid_x, mid_y, text, ha='center', va='center', fontsize=8, 
           rotation=rotation, bbox=dict(boxstyle="round,pad=0.2", facecolor='white', alpha=0.8))

def generate_svg_template():
    """Generate SVG template with updated layout."""
    svg_content = f'''<?xml version="1.0" encoding="UTF-8"?>
<svg width="297mm" height="210mm" viewBox="0 0 297 210" xmlns="http://www.w3.org/2000/svg">
  
  <!-- A4 Page Border -->
  <rect x="5" y="5" width="287" height="200" fill="none" stroke="black" stroke-width="1"/>
  
  <!-- Title -->
  <text x="148.5" y="15" text-anchor="middle" font-family="Arial" font-size="12" font-weight="bold">
    ChoreTracker A4 Template - UPDATED LAYOUT
  </text>
  <text x="148.5" y="27" text-anchor="middle" font-family="Arial" font-size="10">
    Wire holes: 2×27mm @ 1mm from right edge. Bottom row moved down.
  </text>
  
  <!-- BUTTONS ROW 1 (Top) -->'''

    # Generate buttons row 1
    for i in range(4):
        x = MARGIN_LEFT + i * (DISPLAY_WIDTH + DISPLAY_SPACING) + DISPLAY_WIDTH/2 - SQUARE_BUTTON_SIZE/2
        svg_content += f'''
  
  <!-- Button {i} -->
  <g id="button{i}">
    <rect x="{x}" y="{BUTTONS_ROW1_Y}" width="18" height="18" fill="white" stroke="purple" stroke-width="2"/>
    <circle cx="{x + 9}" cy="{BUTTONS_ROW1_Y + 9}" r="8" fill="none" stroke="purple" stroke-width="1" stroke-dasharray="2,2"/>
    <text x="{x + 9}" y="{BUTTONS_ROW1_Y + 12}" text-anchor="middle" font-family="Arial" font-size="6">B{i}</text>
  </g>'''

    svg_content += '''
  
  <!-- SCREENS ROW 1 -->'''

    # Generate displays row 1
    for i in range(4):
        x = MARGIN_LEFT + i * (DISPLAY_WIDTH + DISPLAY_SPACING)
        svg_content += f'''
  
  <!-- Display {i} -->
  <g id="display{i}">
    <rect x="{x}" y="{DISPLAYS_ROW1_Y}" width="{DISPLAY_WIDTH}" height="{DISPLAY_HEIGHT}" fill="white" stroke="blue" stroke-width="2"/>
    <text x="{x + DISPLAY_WIDTH/2}" y="{DISPLAYS_ROW1_Y + DISPLAY_HEIGHT/2 + 3}" text-anchor="middle" font-family="Arial" font-size="8" font-weight="bold">D{i}</text>
    <!-- Mounting holes (1.5mm from edges) -->
    <circle cx="{x + 1.5}" cy="{DISPLAYS_ROW1_Y + 1.5}" r="1" fill="white" stroke="red" stroke-width="1"/>
    <circle cx="{x + DISPLAY_WIDTH - 1.5}" cy="{DISPLAYS_ROW1_Y + 1.5}" r="1" fill="white" stroke="red" stroke-width="1"/>
    <circle cx="{x + 1.5}" cy="{DISPLAYS_ROW1_Y + DISPLAY_HEIGHT - 1.5}" r="1" fill="white" stroke="red" stroke-width="1"/>
    <circle cx="{x + DISPLAY_WIDTH - 1.5}" cy="{DISPLAYS_ROW1_Y + DISPLAY_HEIGHT - 1.5}" r="1" fill="white" stroke="red" stroke-width="1"/>
    <!-- Wire hole (2×27mm, 1mm from right edge) -->
    <rect x="{x + DISPLAY_WIDTH - 2}" y="{DISPLAYS_ROW1_Y + 1}" width="2" height="27" fill="lightgreen" stroke="green" stroke-width="2" opacity="0.5"/>
  </g>'''

    # Add select buttons
    select_button1_x = MARGIN_LEFT + DISPLAY_WIDTH/2
    select_button2_x = MARGIN_LEFT + 3 * (DISPLAY_WIDTH + DISPLAY_SPACING) + DISPLAY_WIDTH/2
    
    svg_content += f'''
  
  <!-- SELECT BUTTONS -->
  
  <!-- Row 1 Select Button -->
  <circle cx="{select_button1_x}" cy="{SELECT_BUTTON1_Y}" r="14.5" fill="white" stroke="orange" stroke-width="3"/>
  <circle cx="{select_button1_x}" cy="{SELECT_BUTTON1_Y}" r="14.5" fill="none" stroke="orange" stroke-width="1" stroke-dasharray="3,3"/>
  <text x="{select_button1_x}" y="{SELECT_BUTTON1_Y + 3}" text-anchor="middle" font-family="Arial" font-size="8" font-weight="bold">R1</text>
  
  <!-- Row 2 Select Button -->
  <circle cx="{select_button2_x}" cy="{SELECT_BUTTON2_Y}" r="14.5" fill="white" stroke="orange" stroke-width="3"/>
  <circle cx="{select_button2_x}" cy="{SELECT_BUTTON2_Y}" r="14.5" fill="none" stroke="orange" stroke-width="1" stroke-dasharray="3,3"/>
  <text x="{select_button2_x}" y="{SELECT_BUTTON2_Y + 3}" text-anchor="middle" font-family="Arial" font-size="8" font-weight="bold">R2</text>
  
  <!-- SCREENS ROW 2 -->'''

    # Generate displays row 2
    for i in range(4):
        display_num = i + 4
        x = MARGIN_LEFT + i * (DISPLAY_WIDTH + DISPLAY_SPACING)
        svg_content += f'''
  
  <!-- Display {display_num} -->
  <g id="display{display_num}">
    <rect x="{x}" y="{DISPLAYS_ROW2_Y}" width="{DISPLAY_WIDTH}" height="{DISPLAY_HEIGHT}" fill="white" stroke="blue" stroke-width="2"/>
    <text x="{x + DISPLAY_WIDTH/2}" y="{DISPLAYS_ROW2_Y + DISPLAY_HEIGHT/2 + 3}" text-anchor="middle" font-family="Arial" font-size="8" font-weight="bold">D{display_num}</text>
    <!-- Mounting holes -->
    <circle cx="{x + 1.5}" cy="{DISPLAYS_ROW2_Y + 1.5}" r="1" fill="white" stroke="red" stroke-width="1"/>
    <circle cx="{x + DISPLAY_WIDTH - 1.5}" cy="{DISPLAYS_ROW2_Y + 1.5}" r="1" fill="white" stroke="red" stroke-width="1"/>
    <circle cx="{x + 1.5}" cy="{DISPLAYS_ROW2_Y + DISPLAY_HEIGHT - 1.5}" r="1" fill="white" stroke="red" stroke-width="1"/>
    <circle cx="{x + DISPLAY_WIDTH - 1.5}" cy="{DISPLAYS_ROW2_Y + DISPLAY_HEIGHT - 1.5}" r="1" fill="white" stroke="red" stroke-width="1"/>
    <!-- Wire hole -->
    <rect x="{x + DISPLAY_WIDTH - 2}" y="{DISPLAYS_ROW2_Y + 1}" width="2" height="27" fill="lightgreen" stroke="green" stroke-width="2" opacity="0.5"/>
  </g>'''

    svg_content += '''
  
  <!-- BUTTONS ROW 2 (Bottom) -->'''

    # Generate buttons row 2
    for i in range(4):
        button_num = i + 4
        x = MARGIN_LEFT + i * (DISPLAY_WIDTH + DISPLAY_SPACING) + DISPLAY_WIDTH/2 - SQUARE_BUTTON_SIZE/2
        svg_content += f'''
  
  <!-- Button {button_num} -->
  <g id="button{button_num}">
    <rect x="{x}" y="{BUTTONS_ROW2_Y}" width="18" height="18" fill="white" stroke="purple" stroke-width="2"/>
    <circle cx="{x + 9}" cy="{BUTTONS_ROW2_Y + 9}" r="8" fill="none" stroke="purple" stroke-width="1" stroke-dasharray="2,2"/>
    <text x="{x + 9}" y="{BUTTONS_ROW2_Y + 12}" text-anchor="middle" font-family="Arial" font-size="6">B{button_num}</text>
  </g>'''

    # Add print instructions
    svg_content += f'''
  
  <!-- Print Instructions -->
  <text x="10" y="200" font-family="Arial" font-size="10" font-style="italic">
    Print at 100% scale on A4 landscape. Wire holes: 2×27mm @ 1mm from right edge. Verify: displays = {DISPLAY_WIDTH}×{DISPLAY_HEIGHT}mm
  </text>
  
</svg>'''

    # Save SVG file
    with open('choretracker_a4_template_updated.svg', 'w') as f:
        f.write(svg_content)

def main():
    """Generate the corrected template."""
    print("Generating ChoreTracker A4 Template with UPDATED layout...")
    
    # Generate PNG version without legend
    fig, ax = create_template(show_legend=False)
    
    # Save as high-resolution PNG
    output_png = 'template_design_corrected.png'
    fig.savefig(output_png, dpi=300, bbox_inches='tight', facecolor='white')
    print(f"✓ PNG template (no legend) saved: {output_png}")
    
    plt.close(fig)
    
    # Generate SVG version with legend
    print("Generating SVG template...")
    generate_svg_template()
    print(f"✓ SVG template saved: choretracker_a4_template_updated.svg")
    
    print("\n" + "="*60)
    print("CORRECTED TEMPLATE SUMMARY")
    print("="*60)
    print(f"TFT Display Size: {DISPLAY_WIDTH} × {DISPLAY_HEIGHT}mm")
    print(f"Mount Holes: 1.5mm from display edges")
    print(f"Wire Holes: 2×27mm slots, 1mm from right edge of each display")
    print(f"Square Button: 18×18mm surface, {SQUARE_BUTTON_HOLE}mm mount hole") 
    print(f"Round Button: Ø{ROUND_BUTTON_HOLE}mm mount hole")
    print(f"Layout: Bottom row moved down, R1/R2 repositioned for clarity")
    print("="*60)
    print("Usage:")
    print("1. Print at 100% scale on A4 landscape")
    print(f"2. Verify first display measures exactly {DISPLAY_WIDTH}×{DISPLAY_HEIGHT}mm")
    print("3. Wire holes are 2×27mm slots, 1mm from right edge for connectors")
    print("4. Use as template for cutting and drilling")
    print("="*60)

if __name__ == "__main__":
    main() 