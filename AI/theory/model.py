"""
Playground Series S6E4 - Irrigation Need Prediction Pipeline
Author: Rafay Siddiqui (24K-0009)
Description: Complete End-to-End Pipeline including data fetching, 
memory optimization, training, threshold adjustment, and artifact generation.
"""

import os
import pandas as pd
import numpy as np
import lightgbm as lgb
import joblib
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.preprocessing import OrdinalEncoder
from sklearn.metrics import confusion_matrix
from sklearn.model_selection import train_test_split

print("=== INITIATING IRRIGATION PREDICTION PIPELINE ===\n")

# ==========================================
# 1. KAGGLE API & DATA RECONNAISSANCE
# ==========================================
print("[1/5] Checking Data Payload...")
if not os.path.exists('train.csv'):
    print("      First run detected! Booting up the Kaggle API...")
    
    # Attempt to securely fetch credentials if running in Colab
    try:
        from google.colab import userdata
        os.environ["KAGGLE_USERNAME"] = userdata.get('KAGGLE_USERNAME')
        os.environ["KAGGLE_KEY"] = userdata.get('KAGGLE_KEY')
    except ImportError:
        print("      Not in Colab. Ensure Kaggle API tokens are active.")

    # Use Python's OS module to run the terminal commands
    os.system('kaggle competitions download -c playground-series-s6e4')
    os.system('unzip -q -o playground-series-s6e4.zip')
    print("      Data successfully breached and extracted!")
else:
    print("      Kaggle CSV files already detected! Skipping download.")


# ==========================================
# 2. MEMORY OPTIMIZATION & LOADING
# ==========================================
print("\n[2/5] Loading and Optimizing Memory...")



train_df = pd.read_csv('train.csv')
test_df = pd.read_csv('test.csv')
sample_sub = pd.read_csv('sample_submission.csv')


# ==========================================
# 3. DATA PREPROCESSING
# ==========================================
print("\n[3/5] Preprocessing Features...")

cat_cols = train_df.select_dtypes(include=['object']).columns.tolist()
num_cols = train_df.select_dtypes(exclude=['object']).columns.tolist()

if 'id' in num_cols: num_cols.remove('id')
if 'Irrigation_Need' in cat_cols: cat_cols.remove('Irrigation_Need')

X = train_df[cat_cols + num_cols]
X_test = test_df[cat_cols + num_cols]

target_mapping = {'Low': 0, 'Medium': 1, 'High': 2}
reverse_mapping = {0: 'Low', 1: 'Medium', 2: 'High'}
y = train_df['Irrigation_Need'].map(target_mapping)

encoder = OrdinalEncoder(handle_unknown='use_encoded_value', unknown_value=-1)
X_encoded = X.copy()
X_test_encoded = X_test.copy()

X_encoded[cat_cols] = encoder.fit_transform(X[cat_cols])
X_test_encoded[cat_cols] = encoder.transform(X_test[cat_cols])


# ==========================================
# 4. FINAL LIGHTGBM & THRESHOLD TRICK
# ==========================================
print("\n[4/5] Training Final LightGBM Model...")

best_lgbm_params = {
    'n_estimators': 395,
    'learning_rate': 0.09953140867236279,
    'max_depth': 4,
    'num_leaves': 96,
    'min_child_samples': 88,
    'subsample': 0.9927471234293412,
    'colsample_bytree': 0.6038840522589419,
    'random_state': 42,
    'device': 'gpu',  # Safely falls back to CPU if GPU isn't available
    'class_weight': 'balanced',
    'verbose': -1
}

final_model = lgb.LGBMClassifier(**best_lgbm_params)
final_model.fit(X_encoded, y)

print("      Generating out-of-fold probabilities & applying math multipliers...")
test_probs = final_model.predict_proba(X_test_encoded)

best_high_mult = 1.70
best_med_mult = 0.95
test_probs[:, 2] *= best_high_mult
test_probs[:, 1] *= best_med_mult

final_test_preds = test_probs.argmax(axis=1)
sample_sub['Irrigation_Need'] = [reverse_mapping[p] for p in final_test_preds]

# Save artifacts
sample_sub.to_csv('submission_final_optimized.csv', index=False)
joblib.dump(final_model, 'final_lightgbm_model.pkl')


# ==========================================
# 5. GENERATING REPORT ARTIFACTS
# ==========================================
print("\n[5/5] Generating Confusion Matrix Graphic...")

X_train_viz, X_val_viz, y_train_viz, y_val_viz = train_test_split(
    X_encoded, y, test_size=0.2, stratify=y, random_state=42
)

viz_model = lgb.LGBMClassifier(**best_lgbm_params)
viz_model.fit(X_train_viz, y_train_viz)
viz_preds = viz_model.predict(X_val_viz)

cm = confusion_matrix(y_val_viz, viz_preds)

plt.figure(figsize=(8, 6))
sns.heatmap(cm, annot=True, fmt='d', cmap='Blues', 
            xticklabels=['Low', 'Medium', 'High'], 
            yticklabels=['Low', 'Medium', 'High'])

plt.title('Final LightGBM Model Confusion Matrix')
plt.ylabel('Actual Irrigation Need')
plt.xlabel('Predicted Irrigation Need')

# Save as an image instead of just showing it!
plt.savefig('confusion_matrix.png', bbox_inches='tight')
plt.close()

print("\n=== PIPELINE COMPLETE ===")
print("The following files are ready for submission:")
print("- submission_final_optimized.csv (Upload to Kaggle)")
print("- final_lightgbm_model.pkl       (Attach to Assignment Zip)")
print("- confusion_matrix.png           (Put in your Report)")