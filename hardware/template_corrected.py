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
SELECT_BUTTONS_Y = 90    # Select buttons between rows
DISPLAYS_ROW2_Y = 115    # Second display row
BUTTONS_ROW2_Y = 150     # Bottom buttons row

# Margins  
MARGIN_LEFT = 15
MARGIN_TOP = 30

def create_template():
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
            
            # Pin holes (left side of display for back connector)
            pin_x = x - 5  # 5mm to the left of display
            pin_y_center = y + DISPLAY_HEIGHT/2
            pin_positions = [
                (pin_x, pin_y_center - 3),  # Top pin
                (pin_x, pin_y_center),      # Middle pin
                (pin_x, pin_y_center + 3)   # Bottom pin
            ]
            
            for px, py in pin_positions:
                pin_hole = Circle((px, py), 1.5, linewidth=2, edgecolor='green', facecolor='white')
                ax.add_patch(pin_hole)
            
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
    select_button = Circle((select_button1_x, SELECT_BUTTONS_Y), ROUND_BUTTON_HOLE/2,
                         linewidth=3, edgecolor='orange', facecolor='white')
    ax.add_patch(select_button)
    
    mount_outline = Circle((select_button1_x, SELECT_BUTTONS_Y), ROUND_BUTTON_HOLE/2,
                         linewidth=1, edgecolor='orange', facecolor='none', linestyle='--')
    ax.add_patch(mount_outline)
    
    ax.text(select_button1_x, SELECT_BUTTONS_Y, 'R1',
           ha='center', va='center', fontweight='bold', fontsize=10)
    ax.text(select_button1_x, SELECT_BUTTONS_Y + ROUND_BUTTON_HOLE/2 + 8, f'{ROUND_BUTTON_HOLE}mm hole',
           ha='center', va='center', fontsize=8)
    
    # Row 2 select button (at screen 4 position)
    select_button = Circle((select_button2_x, SELECT_BUTTONS_Y), ROUND_BUTTON_HOLE/2,
                         linewidth=3, edgecolor='orange', facecolor='white')
    ax.add_patch(select_button)
    
    mount_outline = Circle((select_button2_x, SELECT_BUTTONS_Y), ROUND_BUTTON_HOLE/2,
                         linewidth=1, edgecolor='orange', facecolor='none', linestyle='--')
    ax.add_patch(mount_outline)
    
    ax.text(select_button2_x, SELECT_BUTTONS_Y, 'R2',
           ha='center', va='center', fontweight='bold', fontsize=10)
    ax.text(select_button2_x, SELECT_BUTTONS_Y + ROUND_BUTTON_HOLE/2 + 8, f'{ROUND_BUTTON_HOLE}mm hole',
           ha='center', va='center', fontsize=8)
    
    # Legend
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
    
    # Pin hole legend (updated text)
    pin_legend1 = Circle((legend_x + 10, legend_y + 38), 1.5, 
                         linewidth=2, edgecolor='green', facecolor='white')
    pin_legend2 = Circle((legend_x + 14, legend_y + 38), 1.5, 
                         linewidth=2, edgecolor='green', facecolor='white')
    ax.add_patch(pin_legend1)
    ax.add_patch(pin_legend2)
    ax.text(legend_x + 30, legend_y + 38, 'Pin Holes (Ø3mm, back connector)', fontsize=9)
    
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
           'Print at 100% scale on A4 landscape. REDESIGNED: B-D-S-D-B compact layout. Verify: displays = 57.9×29.0mm',
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

def main():
    """Generate the corrected template."""
    print("Generating ChoreTracker A4 Template with REDESIGNED compact layout...")
    
    # Generate PNG version
    fig, ax = create_template()
    
    # Save as high-resolution PNG
    output_png = 'template_design_corrected.png'
    fig.savefig(output_png, dpi=300, bbox_inches='tight', facecolor='white')
    print(f"✓ PNG template saved: {output_png}")
    
    plt.close(fig)
    
    print("\n" + "="*60)
    print("CORRECTED TEMPLATE SUMMARY")
    print("="*60)
    print(f"TFT Display Size: {DISPLAY_WIDTH} × {DISPLAY_HEIGHT}mm")
    print(f"Mount Holes: 1.5mm from display edges")
    print(f"Pin Holes: Left side for back connectors (Ø3mm)")
    print(f"Square Button: 18×18mm surface, {SQUARE_BUTTON_HOLE}mm mount hole") 
    print(f"Round Button: Ø{ROUND_BUTTON_HOLE}mm mount hole")
    print(f"Layout: Displays moved left for button clearance")
    print("="*60)
    print("Usage:")
    print("1. Print at 100% scale on A4 landscape")
    print(f"2. Verify first display measures exactly {DISPLAY_WIDTH}×{DISPLAY_HEIGHT}mm")
    print("3. Pin holes are for back-side connectors")
    print("4. Use as template for cutting and drilling")
    print("="*60)

if __name__ == "__main__":
    main() 