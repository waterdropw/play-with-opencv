#!/usr/bin/env python3
# -*- coding: utf-8 -*-


# To add a new cell, type '# %%'
# To add a new markdown cell, type '# %% [markdown]'
# %%
# from IPython import get_ipython
# get_ipython().run_line_magic('matplotlib', 'inline')

import sys
import os
import pandas as pd
import matplotlib.pyplot as plt

# %%

def split_group(datamap):
    ''' Split group by the last part of name
    such as BM_memcpy_naive/BM_memcpy_fast will be the same group
    TestCase name rule: BM_testname_tagname
    '''
    categories = []
    groups = {}
    print('Groups: ')
    for cat in datamap.keys():
        categories.append(cat)
        gname = cat[:cat.rfind('_')]
        cname = cat[cat.rfind('_')+1:]
        if not gname in groups:
            cat_map = {cat: datamap[cat]}
            groups[gname] = cat_map
            print('\t{}'.format(gname))
        else:
            groups[gname][cat] = datamap[cat]
        print('\t\t{}'.format(cname))

    return groups


def prepare(ifname):
    if not ifname.endswith('.csv'):
        print('Just CSV file supported!')
        return
    
    dirname = os.path.dirname(ifname)
    basename = os.path.basename(ifname)
    input_file = ifname
    tmp_file = os.path.join(dirname, 'tmp_file.csv')

    with open(ifname, 'r') as f, open(tmp_file, 'w') as of:
        is_header = True
        for line in f:
            if line.startswith('name'):
                is_header = False
            if is_header:
                print(line, end='') # line content has '\n' already
            else:
                of.write(line)

    orig_data = pd.read_csv(tmp_file)
    print(orig_data.info())

    orig_data.drop(['time_unit', 'iterations', 'bytes_per_second', 'items_per_second', 'label', 'error_occurred', 'error_message', 'branch-miss-rate','branch-hit-rate'], axis='columns', inplace=True, errors='ignore')

    names = [ n.split('/')[0] for n in orig_data['name'] ]
    xcoords = [ n.split('/')[-1] for n in orig_data['name'] ]

    xcoords = sorted(set(xcoords), key=xcoords.index)
    category = sorted(set(names), key=names.index)

    tmp = orig_data.set_index('name')
    data_map = {}
    for cat in category:
        items = tmp.filter(like=cat, axis=0)
        data_map[cat] = items

    return data_map, xcoords


def plot_subgraph(ax, x, ycoords, title, labels, ylabel):
    ax.set_title(title)
    for (y, l) in zip(ycoords, labels):
        ax.plot(x, y, label=l)

    ax.set_xlabel('imgsize')
    ax.set_ylabel(ylabel)
    ax.grid()
    ax.legend()


def plot_group(x, gn, datamap, cols=2):
    category = ''
    items = []
    print('Categories: ')
    for cat in datamap.keys():
        print('\t{}'.format(cat))
        category = cat
    print('Total {} categories'.format(len(datamap)))

    print('Items: ')
    for item, _ in datamap[category].iteritems():
        print('\t{}'.format(item))
        items.append(item)
    print('Total {} items'.format(len(items)))

    rows = (len(items)+1) // cols

    fig = plt.figure(constrained_layout=True, figsize=(20,15))
    gs = fig.add_gridspec(rows, cols)
    axes = []
    for r in range(rows):
        for c in range(cols):
            axes.append(fig.add_subplot(gs[r,c]))
    index = 0   
    for item in items:
        ycoords = []
        labels = []
        for cat, data in datamap.items():
            labels.append(cat)
            ycoords.append(data[item])

        plot_subgraph(axes[index], x, ycoords, item, labels, item)
        index += 1

    fig.savefig('output/{}.jpg'.format(gn))

    # fig.show()

def plot(x, groups, cols=2):
    for gn, datamap in groups.items():
        plot_group(x, gn, datamap, cols)


# %%
if __name__ == '__main__':
    if len(sys.argv) > 1:
        input_file = sys.argv[1]
    else:
        input_file = 'output/bm_res.csv'

    data, x = prepare(input_file)
    groups = split_group(data)

    plot(x, groups)


# %%
