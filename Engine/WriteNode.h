/* ***** BEGIN LICENSE BLOCK *****
 * This file is part of Natron <http://www.natron.fr/>,
 * Copyright (C) 2016 INRIA and Alexandre Gauthier-Foichat
 *
 * Natron is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Natron is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Natron.  If not, see <http://www.gnu.org/licenses/gpl-2.0.html>
 * ***** END LICENSE BLOCK ***** */

#ifndef Engine_WriteNode_h
#define Engine_WriteNode_h

// ***** BEGIN PYTHON BLOCK *****
// from <https://docs.python.org/3/c-api/intro.html#include-files>:
// "Since Python may define some pre-processor definitions which affect the standard headers on some systems, you must include Python.h before any standard headers are included."
#include <Python.h>
// ***** END PYTHON BLOCK *****

#include "Global/Macros.h"
#include "Engine/OutputEffectInstance.h"


#define kNatronWriteNodeParamEncodingPluginChoice "encodingPluginChoice"
#define kNatronWriteNodeParamEncodingPluginID "encodingPluginID"

#define kNatronWriteParamFrameStep "frameIncr"
#define kNatronWriteParamFrameStepLabel "Frame Increment"
#define kNatronWriteParamFrameStepHint "The number of frames the timeline should step before rendering the new frame. " \
"If 1, all frames will be rendered, if 2 only 1 frame out of 2, " \
"etc. This number cannot be less than 1."


NATRON_NAMESPACE_ENTER;

/**
 * @brief A wrapper around all OpenFX Writers nodes so that to the user they all appear under a single Write node that has a dynamic
 * settings panel.
 * Every method forwards to the corresponding OpenFX plug-in.
 **/
struct WriteNodePrivate;
class WriteNode : public OutputEffectInstance
{
public:
    
    static EffectInstance* BuildEffect(NodePtr n)
    {
        return new WriteNode(n);
    }
    
    /**
     * @brief Returns if the given plug-in is compatible with this WriteNode container.
     * by default all nodes which inherits GenericWriter in OpenFX are.
     **/
    static bool isBundledWriter(const std::string& pluginID);
    
    WriteNode(NodePtr n);
    
    virtual ~WriteNode();
    
    NodePtr getEmbeddedReader() const;
    
    void setEmbeddedWriter(const NodePtr& node);
    
    virtual bool isWriter() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual bool isVideoWriter() const OVERRIDE FINAL WARN_UNUSED_RETURN ;
    virtual bool isGenerator() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual bool isOutput() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    
    virtual bool isMultiPlanar() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual bool isViewAware() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    
    virtual bool supportsTiles() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual bool supportsMultiResolution() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual bool supportsMultipleClipsBitDepth() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual RenderSafetyEnum renderThreadSafety() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual bool getCanTransform() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual SequentialPreferenceEnum getSequentialPreference() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual EffectInstance::ViewInvarianceLevel isViewInvariant() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual EffectInstance::PassThroughEnum isPassThroughForNonRenderedPlanes() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    
    
    virtual bool getCreateChannelSelectorKnob() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual bool isHostChannelSelectorSupported(bool* defaultR,bool* defaultG, bool* defaultB, bool* defaultA) const OVERRIDE WARN_UNUSED_RETURN;
    
    
    virtual int getMajorVersion() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual int getMinorVersion() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual std::string getPluginID() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual std::string getPluginLabel() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual std::string getPluginDescription() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual void getPluginGrouping(std::list<std::string>* grouping) const OVERRIDE FINAL;
    
    virtual int getMaxInputCount() const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual std::string getInputLabel (int inputNb) const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual bool isInputOptional(int inputNb) const OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual bool isInputMask(int inputNb) const OVERRIDE FINAL WARN_UNUSED_RETURN;
    
    
    virtual void addAcceptedComponents(int inputNb,std::list<ImageComponents>* comps) OVERRIDE FINAL;
    virtual void addSupportedBitDepth(std::list<ImageBitDepthEnum>* depths) const OVERRIDE FINAL;
    
    virtual void onInputChanged(int inputNo) OVERRIDE FINAL;
    
    virtual void purgeCaches() OVERRIDE FINAL;
    
    virtual void onEffectCreated(bool mayCreateFileDialog) OVERRIDE FINAL;
    
private:
    
    virtual StatusEnum getPreferredMetaDatas(NodeMetadata& metadata) OVERRIDE FINAL;
    virtual void onMetaDatasRefreshed(const NodeMetadata& metadata) OVERRIDE FINAL;
    
    virtual void initializeKnobs() OVERRIDE FINAL;
    
    virtual void onKnobsAboutToBeLoaded(const boost::shared_ptr<NodeSerialization>& serialization) OVERRIDE FINAL;
    
    virtual void knobChanged(KnobI* k,
                             ValueChangedReasonEnum reason,
                             ViewSpec view,
                             double time,
                             bool originatedFromMainThread) OVERRIDE FINAL;
    
    virtual StatusEnum getRegionOfDefinition(U64 hash, double time, const RenderScale & scale, ViewIdx view, RectD* rod) OVERRIDE WARN_UNUSED_RETURN;
    
    virtual void getFrameRange(double *first,double *last) OVERRIDE FINAL;
    
    
    virtual void getComponentsNeededAndProduced(double time, ViewIdx view,
                                                EffectInstance::ComponentsNeededMap* comps,
                                                SequenceTime* passThroughTime,
                                                int* passThroughView,
                                                NodePtr* passThroughInput) OVERRIDE;
    
    
    virtual StatusEnum beginSequenceRender(double first,
                                           double last,
                                           double step,
                                           bool interactive,
                                           const RenderScale & scale,
                                           bool isSequentialRender,
                                           bool isRenderResponseToUserInteraction,
                                           bool draftMode,
                                           ViewIdx view) OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual StatusEnum endSequenceRender(double first,
                                         double last,
                                         double step,
                                         bool interactive,
                                         const RenderScale & scale,
                                         bool isSequentialRender,
                                         bool isRenderResponseToUserInteraction,
                                         bool draftMode,
                                         ViewIdx view) OVERRIDE FINAL WARN_UNUSED_RETURN;
    
    virtual StatusEnum render(const RenderActionArgs& args) OVERRIDE WARN_UNUSED_RETURN;
    
    virtual void getRegionsOfInterest(double time,
                                      const RenderScale & scale,
                                      const RectD & outputRoD, //!< full RoD in canonical coordinates
                                      const RectD & renderWindow, //!< the region to be rendered in the output image, in Canonical Coordinates
                                      ViewIdx view,
                                      RoIMap* ret) OVERRIDE FINAL;
    
    virtual FramesNeededMap getFramesNeeded(double time, ViewIdx view) OVERRIDE WARN_UNUSED_RETURN;
    
    boost::scoped_ptr<WriteNodePrivate> _imp;
};

NATRON_NAMESPACE_EXIT;

#endif // Engine_WriteNode_h
