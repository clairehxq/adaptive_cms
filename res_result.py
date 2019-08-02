import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns


def error(est, tr):
    return 1.0 * np.abs(est-tr) / tr

def error_dt(row):
    return error(row['adaptives'], row['true_count']), error(row['big_cms'], row['true_count']), \
            error(row['cms'], row['true_count']), row['true_count']

def cal_depth(p):
    d = np.log(1/(1-p))
    return d

def cal_prob(d):
    p = 1 - (1/np.exp(d))
    return p

def plot(f='adaptive_res.txt', small_w=256, depth=8, layers=8):    
    adaptives = []
    big_cmss = []
    cmss = []
    true_counts = []
    with open(f, 'r') as f:
    #with open('res_zipf', 'r') as f:
        res = f.readlines()

    for l in res:
        adaptive, big_cms, cms, true_count = l.split(',')
        #keys.append(k1)
        adaptives.append(int(adaptive))
        big_cmss.append(int(big_cms))
        cmss.append(int(cms))
        true_counts.append(int(true_count))

    d = pd.DataFrame(columns=['adaptives',
                             'big_cms',
                             'cms',
                             'true_count'])

    d.adaptives = adaptives
    d.big_cms = big_cmss
    d.cms = cmss
    d.true_count = true_counts
    d.sort_values('true_count', ascending=False, inplace=True)
    d_error = d.apply(error_dt, axis=1)
    d_err = pd.DataFrame()
    d_err['adap'] = list(map(lambda x: x[0], d_error))
    d_err['big_cms'] = list(map(lambda x: x[1], d_error))
    d_err['cms'] = list(map(lambda x: x[2], d_error))

    
    big_w = small_w * layers
    small_epsilon = np.e / small_w
    big_epsilon = np.e / big_w

    s_add_error = small_epsilon * sum(d.true_count)
    b_add_error = big_epsilon * sum(d.true_count)

    d_err['cms_error_bound'] = list(map(lambda x: 1.0 * (s_add_error) / x[3], d_error))
    d_err['bcms_error_bound'] = list(map(lambda x: 1.0 * (b_add_error) / x[3], d_error))

    m_error = np.mean(d_err[:(1<<10)])
    m_error_a = np.mean(d_err)

    d_err[:(1<<10)].plot(title = 'Width {}, depth {}'.format(small_w, 8),\
                        figsize=(15,15),\
                        )
    plt.xlabel('rank')
    plt.ylabel('error rate')
    plt.text(1<<9,d_err[:1<<10].max(axis=0 ).max(), 
             'topk mean error, adaptive {:0.2f}, cms {:0.2f}, big cms {:0.2f} \
             mean error, adaptive {:0.2f}, cms {:0.2f}, big cms {:0.2f}'.
             format(m_error['adap'], m_error['cms'], m_error['big_cms'],\
                   m_error_a['adap'], m_error_a['cms'], m_error_a['big_cms']), ha='center')
    
    plt.savefig('figures/{}_width{}_depth{}_rows{}.png'.format(file,small_w, depth, layers))
    return 0

def plot_topk(file='adaptive_res.txt', small_w=256, depth=8, layers=8, topk=10):    
    adaptives = []
    big_cmss = []
    cmss = []
    true_counts = []
    with open(file, 'r') as f:
    #with open('res_zipf', 'r') as f:
        res = f.readlines()

    for l in res:
        adaptive, big_cms, cms, true_count = l.split(',')
        #keys.append(k1)
        adaptives.append(int(adaptive))
        big_cmss.append(int(big_cms))
        cmss.append(int(cms))
        true_counts.append(int(true_count))

    d = pd.DataFrame(columns=['adaptives',
                             'big_cms',
                             'cms',
                             'true_count'])
    
    d.adaptives = adaptives
    d.big_cms = big_cmss
    d.cms = cmss
    d.true_count = true_counts
    d.sort_values('true_count', ascending=False, inplace=True)
    d_error = d.apply(error_dt, axis=1)
    d_err = pd.DataFrame()
    d_err['adapatives'] = list(map(lambda x: x[0], d_error))
    d_err['big_cms'] = list(map(lambda x: x[1], d_error))
    d_err['cms'] = list(map(lambda x: x[2], d_error))
    big_w = small_w * layers
    small_epsilon = np.e / small_w
    big_epsilon = np.e / big_w

    s_add_error = small_epsilon * sum(d.true_count)
    b_add_error = big_epsilon * sum(d.true_count)

    d_err['cms_error_bound'] = list(map(lambda x: 1.0 * (s_add_error) / x[3], d_error))
    d_err['bcms_error_bound'] = list(map(lambda x: 1.0 * (b_add_error) / x[3], d_error))

    m_error = np.mean(d_err[:topk])
    m_error_a = np.mean(d_err)

    plt.figure()
    d_err_p = pd.DataFrame(columns=['error', 'category'])
    d_err_p.loc[:len(d_err),:] = zip(d_err['big_cms'], ['bcms']*len(d_err))
    d_err_p.loc[:len(d_err),:] = zip(d_err['adaptives'], ['adaptives']*len(d_err))
    #d_err_p = d_err.loc[:topk, ['big_cms', 'adapatives']]
    sns.relplot(y='error', data=d_err_p)
    #d_err_p.plot(
        #title = 'Width {}, depth {}, rows {}, top{}'.format(small_w, depth, layers, topk),\
        #figsize=(15,15),
          #  )
    plt.xlabel('rank')
    plt.ylabel('error rate')
    plt.text(topk/2 ,d_err_p.max(axis=1 ).max(), 
             'top{} mean error, adaptive {:0.2f}, cms {:0.2f}, big cms {:0.2f}, \
             mean error, adaptive {:0.2f}, cms {:0.2f}, big cms {:0.2f}'.
             format(topk, m_error['adapatives'], m_error['cms'], m_error['big_cms'],\
                   m_error_a['adapatives'], m_error_a['cms'], m_error_a['big_cms']), ha='center')
    
    #plt.savefig('figures/{}_width{}_depth{}_rows{}_top{}'.format(file, small_w, depth, layers, topk))
    return 0


def read_res(f, p):
    adaptives = []
    big_cmss = []
    cmss = []
    true_counts=[]
    width, depth, row, threshold, memory = list(map(int, f.split('_')))
    small_w = width
    with open(p+f, 'r') as f:
        res = f.readlines()

    for l in res:
        adaptive, ccms, bcms, true_count = l.split(',')
        adaptives.append(float(adaptive))
        cmss.append(float(ccms))
        big_cmss.append(float(bcms))
        true_counts.append(int(true_count))

    d = pd.DataFrame(columns=[
                              'adaptives',
                              'big_cms',
                              'cms',
                              'true_count'])

    d.adaptives = adaptives
    d.big_cms = big_cmss
    d.cms = cmss
    d.true_count = true_counts
    tc = d.true_count
    d_err = pd.DataFrame(columns=['width', 'height', 'row', 'threshold', 'category', 'error'])
    t = pd.DataFrame(list(map(lambda x: [width, depth, row, threshold, 'adaptive', x], d.adaptives)), 
                     columns=['width', 'height', 'row', 'threshold', 'category', 'error' ])
    d_err = d_err.append(t)
    
    t = pd.DataFrame(list(map(lambda x: [width, depth, row, threshold, 'bcms', x], d.big_cms)), 
                     columns=['width', 'height', 'row', 'threshold', 'category', 'error' ])
    d_err = d_err.append(t)
    
    t = pd.DataFrame(list(map(lambda x: [width, depth, row, threshold, 'ccms', x], d.cms)), 
                     columns=['width', 'height', 'row', 'threshold', 'category', 'error' ])
    d_err = d_err.append(t)
    return d_err

def read_res_abs_rel(f, p, topk):
    adaptives = []
    big_cmss = []
    cmss = []
    rel_adaptives = []
    rel_big_cmss = []
    rel_cmss = []
    true_counts=[]
    fold = []
    width, depth, row, threshold, memory = list(map(int, f.split('_')))
    small_w = width
    count=0
    with open(p+f, 'r') as f:
        res = f.readlines()

    for l in res:
        radaptive, rccms, rbcms, adaptive, ccms, bcms, true_count = l.split(',')
        rel_adaptives.append(float(radaptive))
        rel_big_cmss.append(float(rbcms))
        rel_cmss.append(float(rccms))
        adaptives.append(float(adaptive))
        cmss.append(float(ccms))
        big_cmss.append(float(bcms))
        true_counts.append(int(true_count))
        fold.append(int(count/topk))
        count += 1
    d = pd.DataFrame(columns=[
                              'adaptives',
                              'big_cms',
                              'cms',
                              'true_count',
                              'fold'])

    d.adaptives = adaptives
    d.big_cms = big_cmss
    d.cms = cmss
    d.rel_adaptives = rel_adaptives
    d.rel_cms = rel_cmss
    d.rel_big_cms = rel_big_cmss
    d.fold = fold
    d.true_count = true_counts
    tc = d.true_count
    
    d_err = pd.DataFrame(columns=['width', 'height', 'row', 'threshold', 'category', 'error', 'fold'])
    def extend_row(r, category):
        return [width, depth, row, threshold, category, r[0], r[1]]

    t = pd.DataFrame(list(map(functool.partial(extend_row, category='adaptive') ,[d.adaptives, d.fold])), 
                     columns=['width', 'height', 'row', 'threshold', 'category', 'error', 'fold'])
    d_err = d_err.append(t)
    
    t = pd.DataFrame(list(map(extend_row, ['bcms', d.big_cms, d.fold])), 
                     columns=['width', 'height', 'row', 'threshold', 'category', 'error', 'fold'])
    d_err = d_err.append(t)
    
    t = pd.DataFrame(list(map(extend_row, ['ccms', d.cms, d.fold])), 
                     columns=['width', 'height', 'row', 'threshold', 'category', 'error', 'fold'])
    d_err = d_err.append(t)

    # relative error
    d_err_rel = pd.DataFrame(columns=['width', 'height', 'row', 'threshold', 'category', 'error', 'fold'])
    t = pd.DataFrame(list(map(extend_row, ['adaptive', d.rel_adaptives, d.fold])), 
                     columns=['width', 'height', 'row', 'threshold', 'category', 'error', 'fold' ])
    d_err_rel = d_err_rel.append(t)
    
    t = pd.DataFrame(list(map(extend_row,  ['bcms', d.rel_big_cms, d.fold])), 
                     columns=['width', 'height', 'row', 'threshold', 'category', 'error', 'fold' ])
    d_err_rel = d_err_rel.append(t)
    
    t = pd.DataFrame(list(map(extend_row, ['ccms', d.rel_cms, d.fold])), 
                     columns=['width', 'height', 'row', 'threshold', 'category', 'error', 'fold' ])
    d_err_rel = d_err_rel.append(t)
    return d_err, d_err_rel

def read_res_topk(f, k, p):
    adaptives = []
    big_cmss = []
    cmss = []
    true_counts=[]
    width, depth, row, threshold, memory = list(map(int, f.split('_')))
    small_w = width
    with open(p+f, 'r') as f:
        res = f.readlines()

    for l in res:
        adaptive, ccms, bcms, true_count = l.split(',')
        adaptives.append(float(adaptive))
        cmss.append(float(ccms))
        big_cmss.append(float(bcms))
        true_counts.append(int(true_count))

    d = pd.DataFrame(columns=[
                              'adaptives',
                              'big_cms',
                              'cms',
                              'true_count'])

    d.adaptives = adaptives
    d.big_cms = big_cmss
    d.cms = cmss
    d.true_count =true_counts
    d = d.sort_values('true_count', ascending=False).reset_index(drop=True)
    d = d.loc[:(k*10)]
    tc = d.true_count
    d_err = pd.DataFrame(columns=['width', 'height', 'row', 'threshold', 'category', 'error'])
    t = pd.DataFrame(list(map(lambda x: [width, depth, row, threshold, 'adaptive', x], d.adaptives)), 
                     columns=['width', 'height', 'row', 'threshold', 'category', 'error' ])
    d_err = d_err.append(t)
    
    t = pd.DataFrame(list(map(lambda x: [width, depth, row, threshold, 'bcms', x], d.big_cms)), 
                     columns=['width', 'height', 'row', 'threshold', 'category', 'error' ])
    d_err = d_err.append(t)
    
    t = pd.DataFrame(list(map(lambda x: [width, depth, row, threshold, 'ccms', x], d.cms)), 
                     columns=['width', 'height', 'row', 'threshold', 'category', 'error' ])
    d_err = d_err.append(t)
    return d_err


def read_res_row(f, p):
    adaptives = []
    big_cmss = []
    cmss = []
    true_counts=[]
    width, depth, row, threshold, memory = list(map(int, f.split('_')))
    small_w = width
    with open(p+f, 'r') as f:
        res = f.readlines()

    for l in res:
        adaptive, ccms, big_cms, true_count = l.split(',')
        adaptives.append(float(adaptive))
        big_cmss.append(float(big_cms))
        cmss.append(float(ccms))
        true_counts.append(int(true_count))

    d = pd.DataFrame(columns=[
                              'adaptives',
                              'big_cms',
                              'cms',
                              'true_count'])

    d.adaptives = adaptives
    d.big_cms = big_cmss
    d.cms = cmss
    d.true_count = true_counts
    tc = d.true_count
    d_err = pd.DataFrame(columns=['width', 'height', 'row', 'threshold', 'error'])
    t = pd.DataFrame(list(map(lambda x: [width, depth, row, threshold, x], d.adaptives)), 
                     columns=['width', 'height', 'row', 'threshold', 'error' ])

    d_err = d_err.append(t)
    t = pd.DataFrame(list(map(lambda x: [width, depth, 0,  threshold, x], d.big_cms)), 
                     columns=['width', 'height', 'row', 'threshold', 'error' ])
    d_err = d_err.append(t)
    return d_err


def read_res_fix_memory(f, p):
    adaptives = []
    cmss = []
    big_cmss = []
    true_counts = []
    width, depth, row, threshold, memory = list(map(int, f.split('_')))
    small_w = width
    log_r = int(np.log2(row))
    with open(p+f, 'r') as f:
    #with open('res_zipf', 'r') as f:
        res = f.readlines()

    n = len(res[0].split(',')) - 2
    nn=0
    for l in res:
        
        ll = l.split(',')
        adaptive = ll[0]
        big_cms = list(map(float, ll[1:1+n]))
        cms = ll[-2] 
        true_count  = ll[-1]
        #keys.append(k1)
        adaptives.append(float(adaptive))
        big_cmss.append(big_cms)
        cmss.append(float(cms))
        true_counts.append(int(true_count))
        nn += 1
        if nn>1000:
            break
    c = ['adaptives',
          'cms',
          'true_count']
    c.extend(range(n))
    d = pd.DataFrame(columns=c)

    d.adaptives = adaptives
    d.cms = cmss
    d.true_count = true_counts
    for i in range(n):
        d[i] = list(map(lambda x: x[i], big_cmss))
        
    d_err = pd.DataFrame(columns=['width', 'height', 'row', 'threshold', 'category', 'error' ])
    #d.iloc[:, :-1] = 1.0 * (d.iloc[:, :-1]-d.loc[:, 'true_count']) / d.loc[:, "true_count"] #.apply(error_dt, axis=1)
    tc = d.true_count
 
    t = pd.DataFrame(list(map(lambda x: [width, depth, row, threshold, 'adaptive', x], d.adaptives)), 
                     columns=['width', 'height', 'row', 'threshold', 'category', 'error' ])
    d_err = d_err.append(t)
    t = pd.DataFrame(list(map(lambda x: [width, depth, 1, threshold, 'scms', x], d.cms)), 
                     columns=['width', 'height', 'row', 'threshold', 'category', 'error' ])
    d_err = d_err.append(t)
    
    for i in range(n):
        t = pd.DataFrame(list(map(lambda x: [width*(1<<i), depth*(1<<(log_r-i)), row, threshold, 'bcms_{}'.format(i), x],\
                                  d[i])), 
                        columns=['width', 'height', 'row', 'threshold', 'category', 'error' ])
        d_err = d_err.append(t)
    
    return d_err


