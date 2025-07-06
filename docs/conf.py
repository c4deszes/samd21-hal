import sys, os

project = 'SAMD21 Hardware Abstraction Layer'
version = '0.1'
sys.path.append(os.path.abspath("./_ext"))

extensions = [
    "breathe",
    #"sphinx_needs",
    "sphinx_rtd_theme",
    #'jupyter_sphinx',
    #'sphinx.ext.mathjax',
    #'matplotlib.sphinxext.mathmpl',
    #'matplotlib.sphinxext.plot_directive',
    #'sphinx.ext.intersphinx',
    'sphinx.ext.autodoc',
    'sphinx.ext.autosectionlabel',
    'sphinx.ext.doctest',
    'sphinxcontrib.mermaid',
    'sphinxcontrib.kroki',
    'linuxdoc.rstFlatTable',
    #'sphinx.ext.graphviz',
    #'sphinxcontrib.wavedrom'
]

autosectionlabel_prefix_document = True

# Sphinx configuration
html_static_path = ['_static']
html_style = 'css/custom.css'
html_theme = "sphinx_rtd_theme"
html_theme_options = {
    "collapse_navigation" : False
}
html_js_files = ["https://cdn.jsdelivr.net/npm/mermaid/dist/mermaid.min.js"]
source_suffix = '.rst'
master_doc = 'index'

# Breathe configuration
breathe_default_project = "samd21"
breathe_projects = {}

# Sphinx Needs configuration
needs_build_json = True
needs_autoexport = True
needs_builder_filter = "True"
needs_external_needs = [
]
needs_id_required = True
needs_id_regex = "^[a-zA-Z0-9_]{3,}"
needs_template_folder = "_templates"
needs_types = [
    dict(directive="req-sw", title="Software Requirement", prefix="REQ_SW_", color="#BFD8D2", style="node"),
    dict(directive="drs-sw", title="Design Requirement", prefix="DRS_SW_", color="#BFD8D2", style="node"),
    dict(directive="fmea", title="Failure mode", prefix="FMEA_SW_", color="#DF744A", style="node"),
    dict(directive="test", title="Hardware Test Case", prefix="TC_SW_", color="#DCB239", style="node"),
]
# needs_statuses = [
#     dict(name="draft", description="Still under work or waiting for review"),
#     dict(name="reviewed", description="Requirement is valid"),
#     dict(name="accepted", description="Requirement will be taken into account")
# ]
needs_tags = [
    dict(name="security", description="tag for security requirements"),
    dict(name="safety", description="tag for safety requirements")
]
needs_extra_options = [
    "probability",      # valid values: none, remote, occasional, frequent
    "severity",         # valid values: none, minor, critical, catastrophic
    "detection",        # valid values: always, high, low, undetected,
    "mitigation",       # free text
]
needs_extra_links = [
    {
        "option": "satisfies",
        "incoming": "is satisfied by",
        "outgoing": "satisfies"
    },
    {
        "option": "implements",
        "incoming": "is implemented by",
        "outgoing": "implements"
    },
    {
        "option": "tests",
        "incoming": "is tested by",
        "outgoing": "tests"
    },
    {
        "option": "relates",
        "incoming": "is related to",
        "outgoing": "relates"
    },
    {
        "option": "affects",
        "incoming": "is affected by",
        "outgoing": "affects"
    }
]
