#!/usr/bin/env python

""" @file plot_shifted_dS.py

    Created 20 Jul 2015

    Plot the shear signal after accounting for shifting effects.

    ---------------------------------------------------------------------

    Copyright (C) 2015 Bryan R. Gillis

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

import matplotlib
import matplotlib.pyplot as pyplot
import numpy as np
import click

import icebrgpy.bins_funcs as bf
import icebrgpy.columns_reader as cr

matplotlib.rcParams['ps.useafm'] = True
matplotlib.rcParams['pdf.use14corefonts'] = True
matplotlib.rcParams['text.usetex'] = True

@click.command()
@click.option("--input-filename", "input_filename",
              default="/disk2/brg/git/brg_library_testing//Lensing_profile_tester/lensing_test_results.dat",
              help="File containing the shear data to be plotted")
@click.option("--output-folder", "output_filename", default="./plots",
              help="Folder to place generated plots in.")
@click.option("--output-label", "output_label", default="",
              help="Label to prepend to plot filenames.")
@click.option("--figtype", default="png",
              help="Type of image to generate.")


@click.option("--xmin", default=None, type=float, help="Minimum displayed value for x-axis.")
@click.option("--xmax", default=None, type=float, help="Maximum displayed value for x-axis.")
@click.option("--xlog/--xlin", "x_log", default=True, help="Log- or Lin-scale x-axis")

@click.option("--ymin", default=None, type=float, help="Minimum displayed value for x-axis.")
@click.option("--ymax", default=None, type=float, help="Maximum displayed value for x-axis.")
@click.option("--ylog/--ylin", "y_log", default=False, help="Log- or Lin-scale y-axis")

@click.option("--zmin", default=0.2, help="Minimum z value for binning.")
@click.option("--zmax", default=0.7, help="Maximum z value for binning.")
@click.option("--zbins", default=5, help="Number of z bins.")
@click.option("--zlog/--zlin", "z_log", default=False, help="Log- or Lin-scale z bins.")

@click.option("--mmin", default=1e10, help="Minimum mass (in Msun) value for binning.")
@click.option("--mmax", default=1e13, help="Maximum mass (in Msun) value for binning.")
@click.option("--mbins", default=3, help="Number of mass bins.")
@click.option("--mlog/--mlin", "m_log", default=True, help="Log- or Lin-scale m bins.")

@click.option("--figsize", default=(8,4), help="Output figure size.")
@click.option("--labelsize", default=8, help="Size of labels on output figure.")

@click.option("--R-colname", "R_colname", default="R_kpc")
@click.option("--m-colname", "m_colname", default="Sat_mass_msun")
@click.option("--gm-colname", "gm_colname", default="Group_mass_msun")
@click.option("--z-colname", "z_colname", default="Sat_z")
@click.option("--oR-colname", "oR_colname", default="Offset_R_kpc")
@click.option("--dS-colname", "dS_colname", default="DS_sat")
@click.option("--gdS-colname", "gdS_colname", default="DS_group")
@click.option("--tdS-colname", "tdS_colname", default="DS_tot")
@click.option("--odS-colname", "odS_colname", default="DS_sat_offset")
@click.option("--sdS-colname", "sdS_colname", default="DS_sat_shifted")
@click.option("--sndS-colname", "sndS_colname", default="DS_sat_shifted_no_enh")
def main(**kwargs):
    """ Main function for plotting shifted shear signal.
    """
    
    # Magic values
    base_diff_plot_filename = "shifted_sig_diff." + kwargs['figtype']
    
    # Setup
    diff_plot_filename = kwargs['output_label'] + base_diff_plot_filename
    
    # Read in columns from the input file
    reader = cr.columns_reader()
    
    reader.add("R",kwargs['R_colname'])
    reader.add("m",kwargs['m_colname'])
    reader.add("z",kwargs['z_colname'])
    reader.add("dS",kwargs['dS_colname'])
    reader.add("gdS",kwargs['gdS_colname'])
    reader.add("tdS",kwargs['tdS_colname'])
    reader.add("sdS",kwargs['sdS_colname'])
    reader.add("sndS",kwargs['sndS_colname'])
        
    cols = reader.read(kwargs['input_filename'],sort_by=kwargs['R_colname'])
    
    # Bin columns
    binned_cols = []
           
    for _col_i in xrange(reader.num_cols()):
        binned_cols.append([])
    
    # Set up bins
    if(kwargs['z_log']):
        z_bins, z_bins_mids = bf.setup_log_bins(kwargs['zmin'], kwargs['zmax'], kwargs['zbins'])
    else:
        z_bins, z_bins_mids = bf.setup_lin_bins(kwargs['zmin'], kwargs['zmax'], kwargs['zbins'])
        
    if(kwargs['m_log']):
        m_bins, m_bins_mids = bf.setup_log_bins(kwargs['mmin'], kwargs['mmax'], kwargs['mbins'])
    else:
        m_bins, m_bins_mids = bf.setup_lin_bins(kwargs['mmin'], kwargs['mmax'], kwargs['mbins'])
    
    # Bin all values in the table
    for col_i in xrange(reader.num_cols()):
        for _z_i in xrange(kwargs['zbins']):
            new_list = []
            for _m_i in xrange(kwargs['mbins']):
                new_list.append([])
            binned_cols[col_i].append(new_list)

    for elems in zip(*cols):
        
        z = elems[reader.index("z")]
        m = elems[reader.index("m")]
        
        z_i = bf.get_bin_index(z+0.001, z_bins)
        if(z_i<0): continue
        m_i = bf.get_bin_index(m+0.001, m_bins)
        if(m_i<0): continue
        
        for col_i in xrange(reader.num_cols()):
            binned_cols[col_i][z_i][m_i].append(elems[col_i])
        
    # Convert all lists to arrays
    for col_i in xrange(reader.num_cols()):
        for z_i in xrange(kwargs['zbins']):
            for m_i in xrange(kwargs['mbins']):
                binned_cols[col_i][z_i][m_i] = np.array(binned_cols[col_i][z_i][m_i])
                
                
    # Do the diff plot now
    fig = pyplot.figure(figsize=kwargs['figsize'])
    fig.subplots_adjust(wspace=0, hspace=0, bottom=0.1, right=0.95, top=0.95, left=0.12)
    
    ax = fig.add_subplot(1,1,1)
    ax.set_xlabel("Projected Separation (kpc)",labelpad=10)
    ax.set_ylabel(r"$\Delta\Sigma_{\rm t}$ change $(M_{\rm sun}/pc^2)$",labelpad=25)
    ax.set_yticklabels([])
    ax.set_xticklabels([])
    
    for z_i in xrange(kwargs['zbins']):
        
        z = z_bins[z_i][0]
        
        for m_i in xrange(kwargs['mbins']):
        
            m = m_bins[m_i][0]
            
            # Plot this bin
            ax = fig.add_subplot( kwargs['mbins'], kwargs['zbins'], z_i + kwargs['zbins']*m_i + 1)
            if(kwargs['x_log']):
                ax.set_xscale("log", nonposx='clip')
            if(kwargs['y_log']):
                ax.set_yscale("log", nonposy='clip')
            ax.plot( binned_cols[reader.index('R')][z_i][m_i],
                     binned_cols[reader.index('sdS')][z_i][m_i] - 
                        binned_cols[reader.index('dS')][z_i][m_i],
                     'k', linewidth=1,
                     label="Combined")
            ax.plot( binned_cols[reader.index('R')][z_i][m_i],
                     binned_cols[reader.index('sdS')][z_i][m_i] - 
                        binned_cols[reader.index('sndS')][z_i][m_i],
                     'b', linewidth=1,
                     label="Enhancement")
            ax.plot( binned_cols[reader.index('R')][z_i][m_i],
                     binned_cols[reader.index('sndS')][z_i][m_i] - 
                        binned_cols[reader.index('dS')][z_i][m_i],
                     'r', linewidth=1,
                     label="Shifting")
            if(kwargs['y_log']):
                ax.plot( binned_cols[reader.index('R')][z_i][m_i],
                         -(binned_cols[reader.index('sdS')][z_i][m_i] - 
                            binned_cols[reader.index('dS')][z_i][m_i]),
                         'k', linestyle=':', linewidth=1,
                         label="Combined")
                ax.plot( binned_cols[reader.index('R')][z_i][m_i],
                         -(binned_cols[reader.index('sdS')][z_i][m_i] - 
                            binned_cols[reader.index('sndS')][z_i][m_i]),
                         'b', linestyle=':', linewidth=1,
                         label="Enhancement")
                ax.plot( binned_cols[reader.index('R')][z_i][m_i],
                         -(binned_cols[reader.index('sndS')][z_i][m_i] - 
                            binned_cols[reader.index('dS')][z_i][m_i]),
                         'r', linestyle=':', linewidth=1,
                         label="Shifting")
            ax.set_xlim(kwargs['xmin'],kwargs['xmax'])
            ax.set_ylim(kwargs['ymin'],kwargs['ymax'])
            
            # Label the redshift and mass
            xmin = 0.
            xmax = 1.
            ymin = 0.
            ymax = 1.
            ax.text(xmin+(xmax-xmin)*0.95, ymin+(ymax-ymin)*0.9, r"$z$="+ str(z),
                    size=kwargs['labelsize'],
                    horizontalalignment='right', transform = ax.transAxes)
            ax.text(xmin+(xmax-xmin)*0.95, ymin+(ymax-ymin)*0.8, r"$M$=" + "%.1e" % m,
                    size=kwargs['labelsize'],
                    horizontalalignment='right', transform = ax.transAxes)
            
            # set the labels as appropriate
            if(z_i!=0): # Not on the left column
                ax.set_yticklabels([])
                
            if(m_i!=kwargs['mbins']-1): # Not on the bottom row
                ax.set_xticklabels([])
                
            if((z_i==0) and (m_i==kwargs['mbins']-1)): # bottom-left
#                 ax.set_yticks([0.001, 0.01])
#                 ax.set_yticklabels([0.001, 0.01],fontsize=kwargs['labelsize'])
#                 ax.set_xticks([0, 500,1000,1500,2000])
#                 ax.set_xticklabels([0, 500,1000,1500,2000],fontsize=kwargs['labelsize'])
                continue
                
#             if(z_i==0): # left column
#                 ax.set_yticks([0.001, 0.01])
#                 ax.set_yticklabels([0.001, 0.01],fontsize=kwargs['labelsize'])
                
#             if(m_i==kwargs['mbins']-1): # bottom row
#                 ax.set_xticks([500,1000,1500,2000])
#                 ax.set_xticklabels([500,1000,1500,2000],fontsize=kwargs['labelsize'])
    
    
    # Save the figure
    pyplot.savefig(diff_plot_filename, format=kwargs['figtype'], bbox_inches="tight", pad_inches=0.05)
        
    # Show the figure
    pyplot.show()

if __name__ == "__main__":
    main()
